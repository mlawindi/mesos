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
#ifndef __STOUT_INTERNAL_WINDOWS_DIRENT_HPP__
#define __STOUT_INTERNAL_WINDOWS_DIRENT_HPP__


#include <stout/windows.hpp>


// Abbreviated version of the POSIX `dirent` struct. cf. specification[1].
//
// [1] http://www.gnu.org/software/libc/manual/html_node/Directory-Entries.html
struct dirent {
  char d_name[MAX_PATH];
  unsigned short d_namlen;
};


// `DIR` is normally an opaque struct in the standard, we expose the
// implementation here because this header is intended for internal use only.
struct DIR
{
  struct dirent curr;
  char *d_name;
  WIN32_FIND_DATA fd;
  HANDLE handle;
};


# ifdef __cplusplus
extern "C" {
# endif


void _freeDir(DIR* directory);
bool _openDirStream(DIR* directory);


// Windows implementation of POSIX standard `opendir`. cf. specification[1].
// NOTE: this is for internal use only! It is marked as `inline` because we
// want to keep stout a header-only, and because we will only use this function
// a handful of times inside Stout.
//
// [1] http://www.gnu.org/software/libc/manual/html_node/Opening-a-Directory.html#Opening-a-Directory
inline DIR* opendir(const char* path)
{
  assert(path != NULL);

  const char unixFolderSeparator = '/';
  const char windowsFolderSeparator = '\\';
  const char windowsDriveSeparator = ':';
  const char wildcard[] = "*";
  const char dirSeparatorAndWildcard[] = "\\*";

  // Allocate space for directory. Be sure to leave room at the end of
  // `directory->d_name` for a directory separator and a wildcard.
  DIR* directory = (DIR*) malloc(sizeof(DIR));

  if (!directory) {
    errno = ENOMEM;
    return NULL;
  }

  directory->d_name =
    (char*) malloc(strlen(path) + strlen(dirSeparatorAndWildcard));

  if (!directory->d_name) {
    errno = ENOMEM;
    free(directory);
    return NULL;
  }

  // Copy path over and append the appropriate postfix.
  strcpy(directory->d_name, path);

  const size_t lastCharInName = directory->d_name[strlen(directory->d_name) - 1];

  if (lastCharInName != unixFolderSeparator &&
      lastCharInName != windowsFolderSeparator &&
      lastCharInName != windowsDriveSeparator) {
    strcat(directory->d_name, dirSeparatorAndWildcard);
  } else {
    strcat(directory->d_name, wildcard);
  }

  if (!_openDirStream(directory)) {
    _freeDir(directory);
  }

  return directory;
}


// Windows implementation of POSIX standard `readdir`. cf. specification[1].
// NOTE: this is for internal use only! It is marked as `inline` because we
// want to keep stout a header-only, and because we will only use this function
// a handful of times inside Stout.
//
// [1] http://www.gnu.org/software/libc/manual/html_node/Reading_002fClosing-Directory.html#Reading_002fClosing-Directory
inline struct dirent* readdir(DIR* directory)
{
  assert(directory != NULL);

  if (!FindNextFile(directory->handle, &directory->fd))
  {
    return NULL;
  }

  strcpy(directory->curr.d_name, directory->fd.cFileName);
  directory->curr.d_namlen = strlen(directory->curr.d_name);

  return &directory->curr;
}


// Windows implementation of POSIX standard `closedir`. cf. specification[1].
// NOTE: this is for internal use only! It is marked as `inline` because we
// want to keep stout a header-only, and because we will only use this function
// a handful of times inside Stout.
//
// [1] http://www.gnu.org/software/libc/manual/html_node/Reading_002fClosing-Directory.html#Reading_002fClosing-Directory
inline int closedir(DIR* directory)
{
  assert(directory != NULL);

  BOOL searchClosed = false;

  if (directory->handle != INVALID_HANDLE_VALUE) {
    searchClosed = FindClose(directory->handle);
  }

  _freeDir(directory);

  // `closedir` expects 0 on success, so we must invert the error code.
  return !searchClosed;
}


inline void _freeDir(DIR* directory)
{
  assert(directory != NULL);

  free(directory->d_name);
  free(directory);
}


inline bool _openDirStream(DIR* directory)
{
  assert(directory != NULL);

  directory->handle = FindFirstFile(directory->d_name, &directory->fd);

  if (directory->handle == INVALID_HANDLE_VALUE)
  {
    errno = ENOENT;
    return false;
  }

  strcpy(directory->curr.d_name, directory->fd.cFileName);
  directory->curr.d_namlen = strlen(directory->curr.d_name);

  return true;
}


# ifdef __cplusplus
}
# endif


#endif // __STOUT_INTERNAL_WINDOWS_DIRENT_HPP__