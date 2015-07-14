#ifndef __PROCESS_ONCE_HPP__
#define __PROCESS_ONCE_HPP__

#include <process/future.hpp>

#include <stout/nothing.hpp>
#include <stout/synchronized.hpp>

namespace process {

// Provides a _blocking_ abstraction that's useful for performing a
// task exactly once.
class Once
{
public:
  Once() : started(false), finished(false)
  {
#if defined(_WIN32)
  // TODO(aclemmer): timespec does not exist on Windows
  throw 99;
#else /* _WIN32 */
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond, NULL);
#endif /* _WIN32 */
  }

  ~Once()
  {
#if defined(_WIN32)
  // TODO(aclemmer): timespec does not exist on Windows
  throw 99;
#else /* _WIN32 */
    pthread_cond_destroy(&cond);
    pthread_mutex_destroy(&mutex);
#endif /* _WIN32 */
  }

  // Returns true if this Once instance has already transitioned to a
  // 'done' state (i.e., the action you wanted to perform "once" has
  // been completed). Note that this BLOCKS until Once::done has been
  // called.
  bool once()
  {
#if defined(_WIN32)
  // TODO(aclemmer): timespec does not exist on Windows
  throw 99;
#else /* _WIN32 */
    bool result = false;

    synchronized (mutex) {
      if (started) {
        while (!finished) {
          pthread_cond_wait(&cond, &mutex);
        }
        result = true;
      } else {
        started = true;
      }
    }

    return result;
#endif /* _WIN32 */
  }

  // Transitions this Once instance to a 'done' state.
  void done()
  {
#if defined(_WIN32)
  // TODO(aclemmer): timespec does not exist on Windows
  throw 99;
#else /* _WIN32 */
    synchronized (mutex) {
      if (started && !finished) {
        finished = true;
        pthread_cond_broadcast(&cond);
      }
    }
#endif /* _WIN32 */
  }

private:
  // Not copyable, not assignable.
  Once(const Once& that);
  Once& operator = (const Once& that);

#if defined(_WIN32)
  // TODO(aclemmer): timespec does not exist on Windows
#else /* _WIN32 */
  pthread_mutex_t mutex;
  pthread_cond_t cond;
#endif /* _WIN32 */
  bool started;
  bool finished;
};

}  // namespace process {

#endif // __PROCESS_ONCE_HPP__
