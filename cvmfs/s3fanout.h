/**
 * This file is part of the CernVM File System.
 */

#ifndef CVMFS_S3FANOUT_H_
#define CVMFS_S3FANOUT_H_

#include <semaphore.h>
#include <poll.h>

#include <vector>
#include <set>
#include <map>
#include <string>
#include <utility>
#include <climits>

#include "duplex_curl.h"
#include "prng.h"
#include "util_concurrency.h"
#include "dns.h"
#include "util.h"

namespace s3fanout {

/**
 * From where to read the data.
 */
enum Origin {
  kOriginMem = 1,
  kOriginPath,
};  // Origin

/**
 * Possible return values.
 */
enum Failures {
  kFailOk = 0,
  kFailLocalIO,
  kFailBadRequest,
  kFailForbidden,
  kFailHostResolve,
  kFailHostConnection,
  kFailNotFound,
  kFailServiceUnavailable,
  kFailOther,
};  // Failures


inline const char *Code2Ascii(const Failures error) {
  const int kNumElems = 9;
  if (error >= kNumElems)
    return "no text available (internal error)";

  const char *texts[kNumElems];
  texts[0] = "S3: OK";
  texts[1] = "S3: local I/O failure";
  texts[2] = "S3: malformed URL (bad request)";
  texts[3] = "S3: forbidden";
  texts[4] = "S3: failed to resolve host address";
  texts[5] = "S3: host connection problem";
  texts[6] = "S3: not found";
  texts[7] = "S3: service not available";
  texts[8] = "S3: unknown network error";

  return texts[error];
}



struct Statistics {
  double transferred_bytes;
  double transfer_time;
  uint64_t num_requests;
  uint64_t num_retries;

  Statistics() {
    transferred_bytes = 0.0;
    transfer_time = 0.0;
    num_requests = 0;
    num_retries = 0;
  }

  std::string Print() const;
};  // Statistics


/**
 * Contains all the information to specify an upload job.
 */
struct JobInfo {
  enum RequestType {
    kReqHead = 0,
    kReqPut,
    kReqDelete,
  };

  Origin origin;
  struct {
    size_t size;
    size_t pos;
    const unsigned char *data;
  } origin_mem;

  const std::string origin_path;
  const std::string access_key;
  const std::string secret_key;
  const std::string bucket;
  const std::string object_key;
  const std::string hostname;
  bool test_and_set;
  void *callback;  // Callback to be called when job is finished
  MemoryMappedFile *mmf;

  // One constructor per destination + head request
  // TODO: Beautify constructors
  JobInfo() { http_headers = NULL; }
  JobInfo(const std::string a, const std::string s, const std::string h,
          const std::string b, const std::string k, const std::string p) :
          origin(kOriginPath),
          origin_path(p),
          access_key(a), secret_key(s), bucket(b), object_key(k), hostname(h)
          { http_headers = NULL;
            test_and_set = false; }
  JobInfo(const std::string a, const std::string s, const std::string h,
          const std::string b, const std::string k,
          const unsigned char *buffer, size_t size) :
          origin(kOriginMem),
          access_key(a), secret_key(s), bucket(b), object_key(k), hostname(h)
          { http_headers = NULL;
            test_and_set = false;
            origin_mem.size = size;
            origin_mem.data = buffer; }
  ~JobInfo() {}

  // Internal state, don't touch
  CURL *curl_handle;
  struct curl_slist *http_headers;
  FILE *origin_file;
  RequestType request;
  Failures error_code;
  unsigned char num_retries;
  unsigned backoff_ms;
};  // JobInfo

struct S3FanOutDnsEntry {
  S3FanOutDnsEntry() : counter(0), dns_name(), ip(), port("80"),
     clist(NULL), sharehandle(NULL) {}
  unsigned int counter;
  std::string dns_name;
  std::string ip;
  std::string port;
  struct curl_slist *clist;
  CURLSH *sharehandle;
};  // S3FanOutDnsEntry

class S3FanoutManager : SingleCopy {
 public:
  S3FanoutManager();
  ~S3FanoutManager();

  void Init(const unsigned max_pool_handles);
  void Fini();
  void Spawn();

  int PushNewJob(JobInfo *info);
  int PopCompletedJobs(std::vector<s3fanout::JobInfo*> *jobs);

  const Statistics &GetStatistics();
  void SetTimeout(const unsigned seconds);
  void GetTimeout(unsigned *seconds);
  void SetRetryParameters(const unsigned max_retries,
                          const unsigned backoff_init_ms,
                          const unsigned backoff_max_ms);

  bool DoSingleJob(JobInfo *info) const;

 private:
  static int CallbackCurlSocket(CURL *easy, curl_socket_t s, int action,
                                void *userp, void *socketp);
  static void *MainUpload(void *data);
  std::vector<s3fanout::JobInfo*> jobs_todo_;
  pthread_mutex_t *jobs_todo_lock_;
  std::vector<s3fanout::JobInfo*> jobs_completed_;
  pthread_mutex_t *jobs_completed_lock_;

  CURL *AcquireCurlHandle() const;
  void ReleaseCurlHandle(JobInfo *info, CURL *handle) const;
  int InitializeDnsSettings(CURL *handle,
                            std::string remote_host) const;
  void InitializeDnsSettingsCurl(CURL *handle, CURLSH *sharehandle,
                                 curl_slist *clist) const;
  Failures InitializeRequest(JobInfo *info, CURL *handle) const;
  void SetUrlOptions(JobInfo *info) const;
  void UpdateStatistics(CURL *handle);
  bool CanRetry(const JobInfo *info);
  void Backoff(JobInfo *info);
  bool VerifyAndFinalize(const int curl_error, JobInfo *info);
  std::string MkAuthoritzation(const std::string &access_key,
                               const std::string &secret_key,
                               const std::string &timestamp,
                               const std::string &content_type,
                               const std::string &request,
                               const std::string &content_md5_base64,
                               const std::string &bucket,
                               const std::string &object_key) const;
  std::string MkUrl(const std::string &host,
                    const std::string &bucket,
                    const std::string &objkey2) const {
    return "http://" + host + "/" + bucket + "/" + objkey2;
  }

  Prng prng_;
  std::set<CURL *> *pool_handles_idle_;
  std::set<CURL *> *pool_handles_inuse_;
  std::set<S3FanOutDnsEntry *> *sharehandles_;
  std::map<CURL *, S3FanOutDnsEntry *> *curl_sharehandles_;
  dns::CaresResolver *resolver;
  uint32_t pool_max_handles_;
  CURLM *curl_multi_;
  std::string *user_agent_;

  pthread_t thread_upload_;
  bool thread_upload_run_;
  atomic_int32 multi_threaded_;

  struct pollfd *watch_fds_;
  uint32_t watch_fds_size_;
  uint32_t watch_fds_inuse_;
  uint32_t watch_fds_max_;

  pthread_mutex_t *lock_options_;
  unsigned opt_timeout_;

  unsigned opt_max_retries_;
  unsigned opt_backoff_init_ms_;
  unsigned opt_backoff_max_ms_;
  bool opt_ipv4_only_;

  unsigned int max_available_jobs_;
  sem_t available_jobs_;

  // Writes and reads should be atomic because reading happens in a different
  // thread than writing.
  Statistics *statistics_;
};  // S3FanoutManager

}  // namespace s3fanout

#endif  // CVMFS_S3FANOUT_H_
