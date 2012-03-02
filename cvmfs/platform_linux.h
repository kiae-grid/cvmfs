/**
 * This file is part of the CernVM File System.
 *
 * Linux specific system/library calls.
 */

#ifndef CVMFS_PLATFORM_LINUX_H_
#define CVMFS_PLATFORM_LINUX_H_

/**
 * Spinlocks are not necessarily part of pthread on all platforms.
 */
#include <pthread.h>

typedef pthread_spinlock_t platform_spinlock;

inline int platform_spinlock_init(platform_spinlock *lock, int pshared) {
  return pthread_spin_init(lock, pshared);
}

inline int platform_spinlock_destroy(platform_spinlock *lock) {
  return pthread_spin_destroy(lock);
}

inline int platform_spinlock_trylock(platform_spinlock *lock) {
  return pthread_spin_trylock(lock);
}


/**
 * File system functions, ensure 64bit versions.
 */
#include <dirent.h>
#include <sys/stat.h>

typedef struct dirent64 platform_dirent64;

inline platform_dirent64 *platform_readdir(DIR *dirp) {
  return readdir64(dirp);
}

typedef struct stat64 platform_stat64;

inline int platform_stat(const char *path, platform_stat64 *buf) {
  return stat64(path, buf);
}

inline int platform_lstat(const char *path, platform_stat64 *buf) {
  return lstat64(path, buf);
}

inline int platform_fstat(int filedes, platform_stat64 *buf) {
  return fstat64(filedes, buf);
}

#endif  // CVMFS_PLATFORM_LINUX_H_