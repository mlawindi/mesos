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
#ifndef __STOUT_THREAD_HPP__
#define __STOUT_THREAD_HPP__

#include <errno.h>
#if !defined(_WIN32)
#include <pthread.h>
#endif /* _WIN32 */
#include <stdio.h> // For perror.

#include <stout/abort.hpp>

template <typename T>
struct ThreadLocal
{
  ThreadLocal()
  {
#if defined(_WIN32)
  // TODO(aclemmer): timespec does not exist on Windows
  throw 99;
#else /* _WIN32 */
    errno = pthread_key_create(&key, NULL);

    if (errno != 0) {
      ABORT(std::string("Failed to create thread local, pthread_key_create: ") +
            strerror(errno));
    }
#endif /* _WIN32 */
  }

  ~ThreadLocal()
  {
#if defined(_WIN32)
  // TODO(aclemmer): timespec does not exist on Windows
  throw 99;
#else /* _WIN32 */
    errno = pthread_key_delete(key);

    if (errno != 0) {
      ABORT("Failed to destruct thread local, pthread_key_delete: " +
            std::string(strerror(errno)));
    }
#endif /* _WIN32 */
  }

  ThreadLocal<T>& operator = (T* t)
  {
#if defined(_WIN32)
  // TODO(aclemmer): timespec does not exist on Windows
  throw 99;
#else /* _WIN32 */
    errno = pthread_setspecific(key, t);

    if (errno != 0) {
      ABORT(std::string("Failed to set thread local, pthread_setspecific: ") +
            strerror(errno));
    }
    return *this;
#endif /* _WIN32 */
  }

  operator T* () const
  {
#if defined(_WIN32)
  // TODO(aclemmer): timespec does not exist on Windows
  throw 99;
#else /* _WIN32 */
    return reinterpret_cast<T*>(pthread_getspecific(key));
#endif /* _WIN32 */
  }

  T* operator -> () const
  {
#if defined(_WIN32)
  // TODO(aclemmer): timespec does not exist on Windows
  throw 99;
#else /* _WIN32 */
    return reinterpret_cast<T*>(pthread_getspecific(key));
#endif /* _WIN32 */
  }

private:
  // Not expecting any other operators to be used (and the rest?).
  bool operator * (const ThreadLocal<T>&) const;
  bool operator == (const ThreadLocal<T>&) const;
  bool operator != (const ThreadLocal<T>&) const;
  bool operator < (const ThreadLocal<T>&) const;
  bool operator > (const ThreadLocal<T>&) const;

#if defined(_WIN32)
  // TODO(aclemmer): timespec does not exist on Windows
#else /* _WIN32 */
  pthread_key_t key;
#endif /* _WIN32 */
};

#endif // __STOUT_THREAD_HPP__
