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

#ifndef __STOUT_OS_WINDOWS_OPEN_HPP__
#define __STOUT_OS_WINDOWS_OPEN_HPP__

#include <sys/types.h>

#include <string>

#include <stout/error.hpp>
#include <stout/nothing.hpp>
#include <stout/try.hpp>

#include <stout/os/close.hpp>


namespace os {

// Windows does not support O_CLOEXEC, but we still want os::open to accept the
// flag, as this will let us simplify code that calls it.
#define O_CLOEXEC 02000000
//#define O_CLOEXEC_UNDEFINED

inline Try<int> open(const std::string& path, int oflag, mode_t mode = 0)
{
  // TODO(hausdorff): remove these checks and implement this!
#ifdef O_CLOEXEC_UNDEFINED
  // Before we passing oflag to ::open, we need to strip the O_CLOEXEC
  // flag since it's not supported.
  bool cloexec = false;
  if ((oflag & O_CLOEXEC) != 0) {
    oflag &= ~O_CLOEXEC;
    cloexec = true;
  }
#endif

  int fd = ::open(path.c_str(), oflag, mode);

  if (fd < 0) {
    return ErrnoError();
  }

  // TODO(hausdorff): remove these checks and implement this!
#ifdef O_CLOEXEC_UNDEFINED
  if (cloexec) {
    Try<Nothing> result = os::cloexec(fd);
    if (result.isError()) {
      os::close(fd);
      return Error("Failed to set cloexec: " + result.error());
    }
  }
#endif

  return fd;
}

} // namespace os {

#endif // __STOUT_OS_WINDOWS_OPEN_HPP__
