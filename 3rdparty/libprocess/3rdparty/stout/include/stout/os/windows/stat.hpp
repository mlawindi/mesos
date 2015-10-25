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
#ifndef __STOUT_OS_WINDOWS_STAT_HPP__
#define __STOUT_OS_WINDOWS_STAT_HPP__

#include <string>

#include <stout/bytes.hpp>
#include <stout/result.hpp>
#include <stout/try.hpp>
#include <stout/windows.hpp>

#include <stout/os/realpath.hpp>


namespace os {
namespace stat {
namespace internal {

typedef struct _REPARSE_DATA_BUFFER {
  ULONG  ReparseTag;
  USHORT  ReparseDataLength;
  USHORT  Reserved;
  union {
    struct {
      USHORT SubstituteNameOffset;
      USHORT SubstituteNameLength;
      USHORT PrintNameOffset;
      USHORT PrintNameLength;
      ULONG Flags;
      WCHAR PathBuffer[1];
    } SymbolicLinkReparseBuffer;
    struct {
      USHORT SubstituteNameOffset;
      USHORT SubstituteNameLength;
      USHORT PrintNameOffset;
      USHORT PrintNameLength;
      WCHAR PathBuffer[1];
    } MountPointReparseBuffer;
    struct {
      UCHAR DataBuffer[1];
    } GenericReparseBuffer;
  };
} REPARSE_DATA_BUFFER;

#define REPARSE_MOUNTPOINT_HEADER_SIZE   8

} // namespace internal {


inline bool isdir(const std::string& path)
{
  Result<std::string> absolutePath = os::realpath(path);

  if (!absolutePath.isSome()) {
    return false;
  }

  struct _stat s;

  if (::_stat(absolutePath.get().c_str(), &s) < 0) {
    return false;
  }
  return S_ISDIR(s.st_mode);
}


inline bool isfile(const std::string& path)
{
  Result<std::string> absolutePath = os::realpath(path);

  if (!absolutePath.isSome()) {
    return false;
  }

  struct _stat s;

  if (::_stat(absolutePath.get().c_str(), &s) < 0) {
    return false;
  }
  return S_ISREG(s.st_mode);
}


// Tests whether a path points at a symbolic link. We make no attempt to return
// a sensible answer for hard links or shortcuts, because internally we use
// only use symlinks.
//
// This turns out to be a very complicated task on Windows. The gist of it is
// that we know that symlinks on Windows are implemented with the Reparse Point
// API, and so the check itself is a matter of:
//
//   1. Opening a file/folder handle for that path, instructing it specifically
//      not to follow the symlinks as is teh default. (NOTE: because there are
//      different handles for files and folders, we use `CreateFile` which will
//      pick the handle for us correctly as long as you pass it the right
//      flag).
//   2. Checking whether the attributes on that handle have the reparse point
//      bit set, which because symlinks are implemented using reparse points,
//      all symlinks do.
//   3. Using `DeviceIoControl` to obtain information about this reparse point,
//      which we can then query to figure out if it's a reparse point that is
//      owned by the symlink filesysem filter driver.
//   4. If it is, then we report that this path does point at a symlink.
//
// NOTE: it may be helpful to consult the following sources if you choose to
// piece through this code yourself:
//
//   * Though helpful, the Boost code that does something similar here is so
//     abstracted as to be not useful, and also wrong:
//     https://svn.boost.org/trac/boost/ticket/4663
//   * The `CreateFile` documentation, buried deep away, tells you which flags
//     you need to invoke to open a handle that can be queried with
//     `DeviceIoControl` (the short answer is that you need to be sure to pass
//     in `OPEN_EXISTING` and `FILE_FLAG_OPEN_REPARSE_POINT` to get a handle
//     for the symlink and not the file the symlink points to; you also need
//     `FILE_FLAG_BACKUP_SEMANTICS` if the path points to a folder):
//     https://msdn.microsoft.com/en-us/library/windows/desktop/aa363858(v=vs.85).aspx
//   * The somewhat opaque documentation to `DeviceIoControl`, which doesn't
//     help much but does give you some context:
//     https://msdn.microsoft.com/en-us/library/windows/desktop/aa363216(v=vs.85).aspx
//   * The documentation that specifically explains how to invoke
//     `DeviceIoControl` to query a file or folder handle for its reparse
//     point data (the short answer is that you need to pass in the flag
//     `FSCTL_GET_REPARSE_POINT`, though it does not mention that you need to
//     somehow get ahold of a header in the device driver kit that holds the
//     definition of `REPARSE_DATA_BUFFER`, and either copy it or reference it
//     directly):
//     https://msdn.microsoft.com/en-us/library/windows/desktop/aa364571(v=vs.85).aspx
//   * The relevant file in boost's FS library, where they also ended up
//     copying the definition of `REPARSE_DATA_BUFFER`:
//     http://www.boost.org/doc/libs/1_46_1/libs/filesystem/v3/src/operations.cpp
inline bool islink(const std::string& path)
{
  // Convert to absolute path because Windows APIs expect it.
  const Result<std::string> absolutePath = os::realpath(path);

  if (!absolutePath.isSome()) {
    return false;
  }

  // Windows has no built-in way to tell whether a path points at a symbolic
  // link; but, we know that symbolic links are implemented with reparse
  // points, so we begin by checking that.
  const DWORD attributes = GetFileAttributes(absolutePath.get().c_str());
  const bool reparseBitSet = (attributes & FILE_ATTRIBUTE_REPARSE_POINT) != 0;

  // NOTE: required for paths that point at files that don't exist; without
  // this, paths could be created in the call to `CreateFile`.
  if (attributes == INVALID_FILE_ATTRIBUTES || !reparseBitSet) {
    return false;
  }

  // NOTE: we use `CreateFile` because folder and file handles are different,
  // and as long as you pick the right flag, it will choose the right type for
  // you. The flag `CreateFile` requires in order to return a directory handle
  // is a magic flag, `FILE_FLAG_BACKUP_SEMANTICS`. It is not clear why, or
  // what this flag means, the documentation[1] only says it's necessary.
  //
  // [1] https://msdn.microsoft.com/en-us/library/windows/desktop/aa363858(v=vs.85).aspx
  const DWORD accessFlags = isdir(absolutePath.get())
    ? FILE_FLAG_OPEN_REPARSE_POINT | FILE_FLAG_BACKUP_SEMANTICS
    : FILE_FLAG_OPEN_REPARSE_POINT;

  const HANDLE handle = CreateFile(
    absolutePath.get().c_str(),
    0,              // Ignored.
    0,              // Ignored.
    NULL,           // Ignored.
    OPEN_EXISTING,  // Open existing symlink.
    accessFlags,    // Open symlink, not the file it points to.
    NULL);          // Ignored.

  if (handle == INVALID_HANDLE_VALUE) {
    return false;
  }

  const DWORD junk = 0;
  internal::REPARSE_DATA_BUFFER* reparsePointData =
    (internal::REPARSE_DATA_BUFFER*)malloc(MAXIMUM_REPARSE_DATA_BUFFER_SIZE);
  const size_t reparsePointDataSize = MAXIMUM_REPARSE_DATA_BUFFER_SIZE;

  // The semantics of this function are: get the reparse data associated with
  // the `handle` of some open directory or file, and that data in
  // `reparsePointData`.
  const BOOL reparseDataObtained = DeviceIoControl(
    handle,                  // handle to file or directory
    FSCTL_GET_REPARSE_POINT, // Gets reparse point data for file/folder handle.
    NULL,                    // Ignored.
    0,                       // Ignored.
    (LPVOID)reparsePointData,
    reparsePointDataSize,
    (LPDWORD)&junk,          // Ignored.
    NULL);                   // Ignored.

  CloseHandle(handle);

  // Check if the reparse point tag says it's a symlink.
  if (reparseDataObtained &&
    reparsePointData->ReparseTag & IO_REPARSE_TAG_SYMLINK) {
    free(reparsePointData);
    return true;
  } else {
    free(reparsePointData);
    return false;
  }
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
  // TODO: call os::realpath here. Figure out whether we need to support
  // symbolic links.

  struct _stat s;

  if (::_stat(path.c_str(), &s) < 0) {
    return ErrnoError("Error invoking stat for '" + path + "'");
  }

  return Bytes(s.st_size);
}


inline Try<long> mtime(const std::string& path)
{
  // TODO: call os::realpath here. Figure out wheter it's a problem we're calling stat in place of lstat.

  struct _stat s;

  if (::_stat(path.c_str(), &s) < 0) {
    return ErrnoError("Error invoking stat for '" + path + "'");
  }

  return s.st_mtime;
}


inline Try<mode_t> mode(const std::string& path)
{
  // TODO: call os::realpath here.

  struct _stat s;

  if (::_stat(path.c_str(), &s) < 0) {
    return ErrnoError("Error invoking stat for '" + path + "'");
  }

  return s.st_mode;
}


inline Try<dev_t> dev(const std::string& path)
{
  // TODO: call os::realpath here

  struct _stat s;

  if (::_stat(path.c_str(), &s) < 0) {
    return ErrnoError("Error invoking stat for '" + path + "'");
  }

  return s.st_dev;
}


inline Try<ino_t> inode(const std::string& path)
{
  // TODO: call os::realpath here.

  struct _stat s;

  if (::_stat(path.c_str(), &s) < 0) {
    return ErrnoError("Error invoking stat for '" + path + "'");
  }

  return s.st_ino;
}

} // namespace stat {
} // namespace os {

#endif // __STOUT_OS_WINDOWS_STAT_HPP__
