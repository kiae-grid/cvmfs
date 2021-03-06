#include "gtest/gtest.h"

#include <unistd.h>

#include "../../cvmfs/uuid.h"
#include "../../cvmfs/util.h"

using namespace std;  // NOLINT

namespace cvmfs {

TEST(T_Uuid, Unique) {
  UniquePtr<Uuid> uuid(Uuid::Create(""));
  UniquePtr<Uuid> uuid2(Uuid::Create(""));
  ASSERT_TRUE(uuid != NULL);
  ASSERT_TRUE(uuid2 != NULL);
  EXPECT_NE(uuid->uuid(), uuid2->uuid());
}


TEST(T_Uuid, Create) {
  string path;
  FILE *f = CreateTempFile("/tmp/cvmfstest", 0600, "w", &path);
  ASSERT_TRUE(f != NULL);
  fclose(f);
  unlink(path.c_str());

  UniquePtr<Uuid> uuid(Uuid::Create(path));
  ASSERT_TRUE(uuid != NULL);
  UnlinkGuard unlink_guard(path);

  f = fopen(path.c_str(), "r");
  ASSERT_TRUE(f != NULL);
  string line;
  GetLineFile(f, &line);
  fclose(f);
  EXPECT_EQ(line, uuid->uuid());
}

TEST(T_Uuid, FromCache) {
  string path;
  FILE *f = CreateTempFile("/tmp/cvmfstest", 0600, "w", &path);
  ASSERT_TRUE(f != NULL);
  fprintf(f, "unique");
  fclose(f);
  UnlinkGuard unlink_guard(path);

  UniquePtr<Uuid> uuid(Uuid::Create(path));
  ASSERT_TRUE(uuid != NULL);
  EXPECT_EQ("unique", uuid->uuid());
}

TEST(T_Uuid, FailWrite) {
  UniquePtr<Uuid> uuid(Uuid::Create("/no/such/path"));
  EXPECT_TRUE(uuid == NULL);
}

TEST(T_Uuid, FailRead) {
  string path;
  FILE *f = CreateTempFile("/tmp/cvmfstest", 0600, "w", &path);
  ASSERT_TRUE(f != NULL);
  fclose(f);
  UnlinkGuard unlink_guard(path);
  UniquePtr<Uuid> uuid(Uuid::Create(path));
  EXPECT_TRUE(uuid == NULL);
}

}  // namespace cvmfs
