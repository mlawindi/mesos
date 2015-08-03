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
#ifndef __STOUT_WINDOWS_HPP__
#define __STOUT_WINDOWS_HPP__

// DEFINITIONS AND CONSTANTS USED FOR WINDOWS COMPAT.
//
// Putting all the "compat" definitions here makes it possible for files
// throughout the codebase to remain relatively free of all the #if's we'd need
// to make them work.
//
// Roughly, the things that should go in this file are definitions and
// declarations that you would not mind:
//   * being in global scope
//   * gloabally available throughout both the Stout codebase, and any code
//     that links to it (such as Mesos).

// Normally defined in unistd.h.
#define STDIN_FILENO 0
#define STDOUT_FILENO 1
#define STDERR_FILENO 2

// TODO(hausdorff): audit whether we really want `write` to be a macro. On the
// one hand it does allow files like abort.hpp to be one file, but on the other
// hand, anyone who includes abort will now have this macro rolling around in
// global scope. cf. -DNOMINMAX for an example of the pain this causes IRL.
#define write(fd, buf, count) _write(fd, buf, count)

// TODO(aclemmer): Not defined on Windows.  This value is temporary.
#define MAXHOSTNAMELEN 64

// Macros associated with ::access, usually defined in unistd.h.
// TODO(hausdorff): see comment above about auditing function macros.
#define access(path, how) _access(path, how)
#define R_OK 0x4
#define W_OK 0x2
#define X_OK 0x0 // No such permission on Windows
#define F_OK 0x0

// Aliases for file access modes.
#include <fcntl.h>
#define O_RDONLY _O_RDONLY
#define O_WRONLY _O_WRONLY
#define O_RDWR _O_RDWR
#define O_CREAT _O_CREAT
#define O_TRUNC _O_TRUNC
#define O_APPEND _O_APPEND
// TODO(josephw): No equivalent for O_NONBLOCK or O_SYNC

// Alias for mkstemp (requires io.h).
// TODO(hausdorff): see comment above about auditing function macros.
#define mkstemp(path) _mktemp_s(path, strlen(path) + 1)

// Alias for realpath.
#define PATH_MAX MAX_PATH
// TODO(hausdorff): see comment above about auditing function macros.
#define realpath(path, resolved) _fullpath(resolved, path, PATH_MAX)

// Alias for mkdir (requires direct.h).
// TODO(hausdorff): see comment above about auditing function macros.
#define mkdir(path, mode) _mkdir(path)

// PERMISSIONS API. cf. MESOS-3176 to track ongoing permissions work.
//
// WHAT WE PROVIDE ON WINDOWS RIGHT NOW:
//   [x] User write permissions
//   [x] User read permissions
//   [ ] User execute permissions
//   [ ] Group permissions of any sort
//   [ ] Other permissions of any sort
//   [x] Knobs to control "fallback" behavior (e.g., we might choose
//       to fall back to user readability when the user passes the
//       group readability flag in, since we currently do not support
//       group readability)
//
// RATIONALE: Windows currently implements two permissions models:
// (1) an extremely primitive permission model it largely inherited
// from DOS, and (2) the Access Control List (ACL) API. Because there
// is no trivial way to map the classic Unix model into the ACL
// model, we have implemented Unix-style permissions in terms of the
// DOS model. The result is the permissions limitations above.
//
// FLAG IMPLEMENTATION: Flags fall into two categories:
//   (1) Flags which exist in both permission models, but which have
//       different names (e.g., `S_IRUSR` and `_S_IREAD`). In this
//       case, we define the Unix name to be the Windows value (e.g.,
//       we define `S_IRUSR` to have the same value as `_S_IREAD`),
//       so that we can pass the Unix name into Windows functions like
//       `_open`.
//   (2) Flags which exist only on Unix (e.g., `S_IXUSR`). Here we
//       define the Unix name to be the value given in the glibc
//       documentation[1], shifted right by 16 bits (since `mode_t`
//       is unsigned short on Unix and int on Windows). We give these
//       flags glibc values to stay consistent, and so that existing
//       calls to functions like `open` do not break when they try to
//       use a flag that doesn't exist on Windows. But, of course,
//       these flags do not affect the execution of these functions.
//
// FLAG STRICTNESS: Because the current implementation does not
// directly support setting or getting group or other permission bits
// on the Windows platform, there is a question of what we should
// fall back to when these flags are passed in to Stout methods.
//
// EXECUTE PERMISSIONS: Because DOS has no notion of "execute
// permissions", we define execute permissions to be read
// permissions. This is not ideal, but it is closest to being
// accurate.
//
// We let the user decide by defining flags that dictate the
// "strictness" of the flag semantics:
//   * STRICT_GROUP_PERMISSIONS: When this is defined, this group
//     permissions flags will not fall back to anything, and will be
//     completely ignored. When not set, group permissions fall back
//     to their user equivalents (e.g., if we pass the flag that sets
//     group write permissions, we fall back to setting user write
//     permissions.
//   * STRICT_OTHER_PERMISSIONS: Same as above, but with other
//     permissions.
//
// [1] http://www.delorie.com/gnu/docs/glibc/libc_288.html


// Corresponds to `mode_t` defined in sys/types.h of the POSIX spec.
// See note above for an explanation of why this is an int instead of
// unsigned short (as is common on *nix).
typedef int mode_t;

// USER PERMISSION FLAGS.
const mode_t S_IRUSR = mode_t(_S_IREAD);  // readable by user
const mode_t S_IWUSR = mode_t(_S_IWRITE); // writeable by user
const mode_t S_IXUSR = S_IRUSR;           // FALLBACK TO USER READ
const mode_t S_IRWXU = S_IRUSR
                     | S_IWUSR
                     | S_IXUSR;

// GROUP PERMISSION FLAGS. Lossy mapping to Windows permissions. See
// note above about FLAG STRICTNESS for explanation.
#ifdef STRICT_GROUP_PERMISSIONS
const mode_t S_IRGRP = 0x00200000;        // nop
const mode_t S_IWGRP = 0x00100000;        // nop
const mode_t S_IXGRP = 0x00080000;        // nop
#else // STRICT_GROUP_PERMISSIONS
const mode_t S_IRGRP = mode_t(_S_IREAD);  // FALLBACK TO USER READ
const mode_t S_IWGRP = mode_t(_S_IWRITE); // FALLBACK TO USER WRITE
const mode_t S_IXGRP = S_IRGRP;           // FALLBACK TO USER READ
#endif // STRICT_GROUP_PERMISSIONS
const mode_t S_IRWXG = S_IRGRP
                     | S_IWGRP
                     | S_IXGRP;

// OTHER PERMISSION FLAGS. Lossy mapping to Windows permissions. See
// note above about FLAG STRICTNESS for explanation.
#ifdef STRICT_OTHER_PERMISSIONS
const mode_t S_IROTH = 0x00040000;        // nop
const mode_t S_IWOTH = 0x00020000;        // nop
const mode_t S_IXOTH = 0x00010000;        // nop
#else // STRICT_OTHER_PERMISSIONS
const mode_t S_IROTH = mode_t(_S_IREAD);  // FALLBACK TO USER READ
const mode_t S_IWOTH = mode_t(_S_IWRITE); // FALLBACK TO USER WRITE
const mode_t S_IXOTH = S_IROTH;           // FALLBACK TO USER READ
#endif // STRICT_OTHER_PERMISSIONS
const mode_t S_IRWXO = S_IROTH
                     | S_IWOTH
                     | S_IXOTH;

// Flags for set-ID-on-exec.
const mode_t S_ISUID = 0x08000000;        // nop
const mode_t S_ISGID = 0x04000000;        // nop
const mode_t S_ISVTX = 0x02000000;        // nop


#endif // __STOUT_WINDOWS_HPP__
