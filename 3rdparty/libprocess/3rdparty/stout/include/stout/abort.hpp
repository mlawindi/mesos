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
#ifndef __STOUT_ABORT_HPP__
#define __STOUT_ABORT_HPP__

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if !defined(_WIN32)
#include <unistd.h>
#endif /* _WIN32 */

// Signal safe abort which prints a message.
#define __STRINGIZE(x) #x
#define _STRINGIZE(x) __STRINGIZE(x)
#define _ABORT_PREFIX "ABORT: (" __FILE__ ":" _STRINGIZE(__LINE__) "): "

#define ABORT(...) _Abort(_ABORT_PREFIX, __VA_ARGS__)

#if defined(MESOS_MSVC)
inline __declspec(noreturn) void _Abort(
    const char *prefix,
    const char *msg)
{
  // TODO(aclemmer): method stubbed out
  throw 99;
}
#elif defined(MESOS_MINGW)
#else /* MESOS_MSVC */
inline __attribute__((noreturn)) void _Abort(
    const char *prefix,
    const char *msg)
{
  // Write the failure message in an async-signal safe manner,
  // assuming strlen is async-signal safe or optimized out.
  // In fact, it is highly unlikely that strlen would be
  // implemented in an unsafe manner:
  // http://austingroupbugs.net/view.php?id=692
  while (write(STDERR_FILENO, prefix, strlen(prefix)) == -1 &&
    errno == EINTR);
  while (msg != NULL &&
    write(STDERR_FILENO, msg, strlen(msg)) == -1 &&
    errno == EINTR);
  abort();
}
#endif /* MESOS_MSVC  */

#if defined(MESOS_MSVC)
inline __declspec(noreturn) void _Abort(
  const char *prefix,
  const std::string &msg) {
  // TODO(aclemmer): method stubbed out
  throw 99;
}
#elif defined(MESOS_MINGW)
#else /* _WIN32 */
inline __attribute__((noreturn)) void _Abort(
  const char *prefix,
  const std::string &msg) {
  _Abort(prefix, msg.c_str());
}
#endif /* _WIN32 */

#endif // __STOUT_ABORT_HPP__
