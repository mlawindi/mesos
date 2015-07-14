/**
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef __STOUT_OS_STAT_HPP__
#define __STOUT_OS_STAT_HPP__

#include <sys/stat.h>
#if defined(_WIN32)
#else /* _WIN32 */
#include <sys/statvfs.h>
#endif /* _WIN32 */

#include <string>

#include <stout/bytes.hpp>
#include <stout/try.hpp>
#include <stout/unreachable.hpp>

namespace os {
namespace stat {

inline bool isdir(const std::string& path)
{
#if defined(MESOS_MSVC)
#else /* MESOS_MSVC */
  struct stat s;

  if (::stat(path.c_str(), &s) < 0) {
    return false;
  }
  return S_ISDIR(s.st_mode);
#endif /* MESOS_MSVC */
}


inline bool isfile(const std::string& path)
{
#if defined(MESOS_MSVC)
#else /* MESOS_MSVC */
  struct stat s;

  if (::stat(path.c_str(), &s) < 0) {
    return false;
  }
  return S_ISREG(s.st_mode);
#endif /* MESOS_MSVC */
}



inline bool islink(const std::string& path)
{
#if defined(_WIN32)
  // TODO(aclemmer): lstat does not exist on Windows
  throw 99;
#else /* _WIN32 */
  struct stat s;

  if (::lstat(path.c_str(), &s) < 0) {
    return false;
  }
  return S_ISLNK(s.st_mode);
#endif /* _WIN32 */
}


// Describes the different semantics supported for the implementation
// of `size` defined below.
enum FollowSymlink
{
  DO_NOT_FOLLOW_SYMLINK,
  FOLLOW_SYMLINK
};


// Returns the size in Bytes of a given file system entry. When
// applied to a symbolic link with `follow` set to
// `DO_NOT_FOLLOW_SYMLINK`, this will return the length of the entry
// name (strlen).
inline Try<Bytes> size(
    const std::string& path,
    const FollowSymlink follow = FOLLOW_SYMLINK)
{
#if defined(_WIN32)
  // TODO(aclemmer): munmap does not exist on Windows
  throw 99;
#else /* _WIN32 */
  struct stat s;

  switch (follow) {
    case DO_NOT_FOLLOW_SYMLINK: {
      if (::lstat(path.c_str(), &s) < 0) {
        return ErrnoError("Error invoking lstat for '" + path + "'");
      }
      break;
    }
    case FOLLOW_SYMLINK: {
      if (::stat(path.c_str(), &s) < 0) {
        return ErrnoError("Error invoking stat for '" + path + "'");
      }
      break;
    }
    default: {
      UNREACHABLE();
    }
  }

  return Bytes(s.st_size);
#endif /* _WIN32 */
}


inline Try<long> mtime(const std::string& path)
{
#if defined(_WIN32)
  // TODO(aclemmer): munmap does not exist on Windows
  throw 99;
#else /* _WIN32 */
  struct stat s;

  if (::lstat(path.c_str(), &s) < 0) {
    return ErrnoError("Error invoking stat for '" + path + "'");
  }

  return s.st_mtime;
#endif /* _WIN32 */
}


#if defined(MESOS_MSVC)
#else /* MESOS_MSVC */
inline Try<mode_t> mode(const std::string& path)
{
  struct stat s;

  if (::stat(path.c_str(), &s) < 0) {
    return ErrnoError("Error invoking stat for '" + path + "'");
  }

  return s.st_mode;
}
#endif /* MESOS_MSVC */


inline Try<dev_t> rdev(const std::string& path)
{
#if defined(MESOS_MSVC)
  // TODO(aclemmer): doesn't work on MSVC
  throw 99;
#else /* MESOS_MSVC */
  struct stat s;

  if (::stat(path.c_str(), &s) < 0) {
    return ErrnoError("Error invoking stat for '" + path + "'");
  }

  if (!S_ISCHR(s.st_mode) && !S_ISBLK(s.st_mode)) {
    return Error("Not a special file: " + path);
  }

  return s.st_rdev;
#endif /* MESOS_MSVC */
}


inline Try<ino_t> inode(const std::string& path)
{
  struct stat s;

  if (::stat(path.c_str(), &s) < 0) {
    return ErrnoError("Error invoking stat for '" + path + "'");
  }

  return s.st_ino;
}

} // namespace stat {
} // namespace os {
#endif // __STOUT_OS_STAT_HPP__
