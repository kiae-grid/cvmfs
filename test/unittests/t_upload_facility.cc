#include <gtest/gtest.h>
#include <unistd.h>

#include "../../cvmfs/util.h"
#include "../../cvmfs/hash.h"
#include "../../cvmfs/file_processing/char_buffer.h"

#include "testutil.h"


using namespace upload;


struct UF_MockStreamHandle : public upload::UploadStreamHandle {
  UF_MockStreamHandle(const callback_t *commit_callback) :
    UploadStreamHandle(commit_callback),
    commits(0), uploads(0)
  {
    ++instances;
  }

  ~UF_MockStreamHandle() {
    --instances;
  }

  int commits;
  int uploads;

  static int instances;
};

int UF_MockStreamHandle::instances = 0;


/**
 * Mocked uploader that just keeps the processing results in memory for later
 * inspection.
 */
class UF_MockUploader : public AbstractMockUploader<UF_MockUploader> {
 public:
  UF_MockUploader(const SpoolerDefinition &spooler_definition) :
    AbstractMockUploader<UF_MockUploader>(spooler_definition),
    initialize_called(false) {}

  upload::UploadStreamHandle* InitStreamedUpload(
                                            const callback_t *callback = NULL) {
    return new UF_MockStreamHandle(callback);
  }

  bool Initialize() {
    AbstractUploader::Initialize();
    initialize_called = true;
    return true;
  }

  void Upload(upload::UploadStreamHandle  *abstract_handle,
              upload::CharBuffer          *buffer,
              const callback_t            *callback = NULL) {
    UF_MockStreamHandle* handle =
                             static_cast<UF_MockStreamHandle*>(abstract_handle);
    handle->uploads++;
    Respond(callback, UploaderResults(0, buffer));
  }

  void FinalizeStreamedUpload(upload::UploadStreamHandle *abstract_handle,
                              const shash::Any            content_hash,
                              const shash::Suffix         hash_suffix) {
    UF_MockStreamHandle* handle =
                             static_cast<UF_MockStreamHandle*>(abstract_handle);
    handle->commits++;
    const UF_MockStreamHandle::callback_t *callback = handle->commit_callback;
    delete handle;
    Respond(callback, UploaderResults(0));
  }

 public:
  bool initialize_called;
};


//
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//


TEST(T_UploadFacility, InitializeAndTearDown) {
  UF_MockUploader *uploader = UF_MockUploader::MockConstruct();

  ASSERT_NE (static_cast<UF_MockUploader*>(NULL), uploader);
  EXPECT_TRUE (uploader->initialize_called);

  sleep(1);
  EXPECT_TRUE (uploader->worker_thread_running);

  uploader->TearDown();
  delete uploader;

  EXPECT_FALSE (uploader->worker_thread_running);
}


//
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//


volatile int chunk_upload_complete_callback_calls = 0;
void ChunkUploadCompleteCallback_T_Callbacks(const UploaderResults &results) {
  EXPECT_EQ (UploaderResults::kChunkCommit,  results.type);
  EXPECT_EQ (0,                              results.return_code);
  EXPECT_EQ ("",                             results.local_path);
  EXPECT_EQ (static_cast<CharBuffer*>(NULL), results.buffer);
  ++chunk_upload_complete_callback_calls;
}

volatile int buffer_upload_complete_callback_calls = 0;
void BufferUploadCompleteCallback_T_Callbacks(const UploaderResults &results) {
  EXPECT_EQ (UploaderResults::kBufferUpload, results.type);
  EXPECT_EQ (0,                              results.return_code);
  EXPECT_EQ ("",                             results.local_path);
  EXPECT_NE (static_cast<CharBuffer*>(NULL), results.buffer);
  EXPECT_LT (size_t(0),                      results.buffer->used_bytes());
  EXPECT_LE (0,                              results.buffer->base_offset());
  ++buffer_upload_complete_callback_calls;
}


TEST(T_UploadFacility, Callbacks) {
  UF_MockUploader *uploader = UF_MockUploader::MockConstruct();

  ASSERT_NE (static_cast<UF_MockUploader*>(NULL), uploader);
  EXPECT_TRUE (uploader->initialize_called);
  EXPECT_EQ (0, chunk_upload_complete_callback_calls);
  EXPECT_EQ (0, buffer_upload_complete_callback_calls);
  EXPECT_EQ (0, UF_MockStreamHandle::instances);

  sleep(1);
  EXPECT_TRUE (uploader->worker_thread_running);

  UploadStreamHandle *handle = uploader->InitStreamedUpload(
      AbstractUploader::MakeCallback(&ChunkUploadCompleteCallback_T_Callbacks));
  ASSERT_NE (static_cast<void*>(NULL), handle);

  EXPECT_EQ (1, UF_MockStreamHandle::instances);

  sleep(1);

  EXPECT_EQ (0, chunk_upload_complete_callback_calls);
  EXPECT_EQ (0, buffer_upload_complete_callback_calls);

  CharBuffer b1(1024);
  b1.SetUsedBytes(1000);
  b1.SetBaseOffset(0);
  uploader->ScheduleUpload(handle, &b1,
    AbstractUploader::MakeCallback(&BufferUploadCompleteCallback_T_Callbacks));

  sleep(1);

  EXPECT_EQ (0, chunk_upload_complete_callback_calls);
  EXPECT_EQ (1, buffer_upload_complete_callback_calls);

  CharBuffer b2(1024);
  b2.SetUsedBytes(1000);
  b2.SetBaseOffset(1000);
  uploader->ScheduleUpload(handle, &b2,
    AbstractUploader::MakeCallback(&BufferUploadCompleteCallback_T_Callbacks));

  sleep(1);

  EXPECT_EQ (0, chunk_upload_complete_callback_calls);
  EXPECT_EQ (2, buffer_upload_complete_callback_calls);

  uploader->ScheduleCommit(handle, shash::Any(), shash::kSuffixNone);

  uploader->WaitForUpload();
  uploader->TearDown();

  EXPECT_EQ (1, chunk_upload_complete_callback_calls);
  EXPECT_EQ (2, buffer_upload_complete_callback_calls);
  EXPECT_EQ (0, UF_MockStreamHandle::instances);

  delete uploader;
}


//
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//

size_t overall_size_ordering = 0;
bool ordering_test_done = false;
void ChunkUploadCompleteCallback_T_Ordering(const UploaderResults &results) {
  ordering_test_done = true;
}

void BufferUploadCompleteCallback_T_Ordering(const UploaderResults &results) {
  EXPECT_EQ (UploaderResults::kBufferUpload, results.type);
  ASSERT_NE (static_cast<CharBuffer*>(NULL), results.buffer);
  EXPECT_LT (size_t(0), results.buffer->used_bytes());

  EXPECT_EQ (static_cast<off_t>(overall_size_ordering), results.buffer->base_offset());
  overall_size_ordering += results.buffer->used_bytes();
}

// Caveat: 'offset' it automatically updated!
CharBuffer* MakeBuffer(const size_t  buffer_size,
                             size_t &offset,
                       const size_t  used_bytes) {
  CharBuffer *buffer = new CharBuffer(buffer_size);
  assert (buffer != NULL);
  buffer->SetUsedBytes(used_bytes);
  buffer->SetBaseOffset(static_cast<off_t>(offset));
  offset += used_bytes;
  return buffer;
}

TEST(T_UploadFacility, DataBlockBasicOrdering) {
  UF_MockUploader *uploader = UF_MockUploader::MockConstruct();

  ASSERT_NE (static_cast<UF_MockUploader*>(NULL), uploader);
  EXPECT_TRUE (uploader->initialize_called);

  sleep(1);
  EXPECT_TRUE (uploader->worker_thread_running);

  UploadStreamHandle *handle = uploader->InitStreamedUpload(
      AbstractUploader::MakeCallback(&ChunkUploadCompleteCallback_T_Ordering));
  ASSERT_NE (static_cast<void*>(NULL), handle);

  std::vector<CharBuffer*> buffers;
  size_t overall_size = 0;
  buffers.push_back(MakeBuffer(1024, overall_size,  384));
  buffers.push_back(MakeBuffer(2048, overall_size, 1024));
  buffers.push_back(MakeBuffer(4096, overall_size, 4096));
  buffers.push_back(MakeBuffer(8192, overall_size, 6172));
  buffers.push_back(MakeBuffer( 768, overall_size,  128));
  buffers.push_back(MakeBuffer(2000, overall_size, 1921));
  buffers.push_back(MakeBuffer(9999, overall_size, 9999));
  buffers.push_back(MakeBuffer( 100, overall_size,   10));
  buffers.push_back(MakeBuffer(4096, overall_size,  128));
  buffers.push_back(MakeBuffer(4627, overall_size, 1950));
  ASSERT_EQ (size_t(25812), overall_size);

  std::vector<CharBuffer*>::const_iterator i    = buffers.begin();
  std::vector<CharBuffer*>::const_iterator iend = buffers.end();
  for (; i != iend; ++i) {
    uploader->ScheduleUpload(handle, *i,
      AbstractUploader::MakeCallback(&BufferUploadCompleteCallback_T_Ordering));
  }

  uploader->ScheduleCommit(handle, shash::Any(), shash::kSuffixNone);
  uploader->WaitForUpload();
  uploader->TearDown();

  EXPECT_EQ (overall_size, overall_size_ordering);
  EXPECT_TRUE (ordering_test_done);

  delete uploader;
}

TEST(T_UploadFacility, InitDtorRace) {
  UF_MockUploader *uploader = UF_MockUploader::MockConstruct();

  ASSERT_NE (static_cast<UF_MockUploader*>(NULL), uploader);
  EXPECT_TRUE (uploader->initialize_called);

  uploader->WaitForUpload();
  uploader->TearDown();
  delete uploader;
}
