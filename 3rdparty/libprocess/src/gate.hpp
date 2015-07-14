#ifndef __GATE_HPP__
#define __GATE_HPP__

// TODO(benh): Build implementation directly on-top-of futex's for Linux.

#include <stout/synchronized.hpp>

class Gate
{
public:
  typedef intptr_t state_t;

private:
  int waiters;
  state_t state;
#if defined(MESOS_MSVC)
#else /* MESOS_MSVC */
  pthread_mutex_t mutex;
  pthread_cond_t cond;
#endif /* MESOS_MSVC */

public:
  Gate() : waiters(0), state(0)
  {
#if defined(MESOS_MSVC)
    // TODO(aclemmer): doesn't work on MSVC
    throw 99;
#else /* MESOS_MSVC */
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond, NULL);
#endif /* MESOS_MSVC */
  }

  ~Gate()
  {
#if defined(MESOS_MSVC)
    // TODO(aclemmer): doesn't work on MSVC
    throw 99;
#else /* MESOS_MSVC */
    pthread_cond_destroy(&cond);
    pthread_mutex_destroy(&mutex);
#endif /* MESOS_MSVC */
  }

  // Signals the state change of the gate to any (at least one) or
  // all (if 'all' is true) of the threads waiting on it.
  void open(bool all = true)
  {
#if defined(MESOS_MSVC)
    // TODO(aclemmer): doesn't work on MSVC
    throw 99;
#else /* MESOS_MSVC */
    synchronized (mutex) {
      state++;
      if (all) {
        pthread_cond_broadcast(&cond);
      } else {
        pthread_cond_signal(&cond);
      }
    }
#endif /* MESOS_MSVC */
  }

  // Blocks the current thread until the gate's state changes from
  // the current state.
  void wait()
  {
#if defined(MESOS_MSVC)
    // TODO(aclemmer): doesn't work on MSVC
    throw 99;
#else /* MESOS_MSVC */
    synchronized (mutex) {
      waiters++;
      state_t old = state;
      while (old == state) {
        pthread_cond_wait(&cond, &mutex);
      }
      waiters--;
    }
#endif /* MESOS_MSVC */
  }

  // Gets the current state of the gate and notifies the gate about
  // the intention to wait for its state change.
  // Call 'leave()' if no longer interested in the state change.
  state_t approach()
  {
#if defined(MESOS_MSVC)
    // TODO(aclemmer): doesn't work on MSVC
    throw 99;
#else /* MESOS_MSVC */
    synchronized (mutex) {
      waiters++;
      return state;
    }
#endif /* MESOS_MSVC */
  }

  // Blocks the current thread until the gate's state changes from
  // the specified 'old' state. The 'old' state can be obtained by
  // calling 'approach()'.
  void arrive(state_t old)
  {
#if defined(MESOS_MSVC)
    // TODO(aclemmer): doesn't work on MSVC
    throw 99;
#else /* MESOS_MSVC */
    synchronized (mutex) {
      while (old == state) {
        pthread_cond_wait(&cond, &mutex);
      }

      waiters--;
    }
#endif /* MESOS_MSVC */
  }

  // Notifies the gate that a waiter (the current thread) is no
  // longer waiting for the gate's state change.
  void leave()
  {
#if defined(MESOS_MSVC)
    // TODO(aclemmer): doesn't work on MSVC
    throw 99;
#else /* MESOS_MSVC */
    synchronized (mutex) {
      waiters--;
    }
#endif /* MESOS_MSVC */
  }

  // Returns true if there is no one waiting on the gate's state
  // change.
  bool empty()
  {
#if defined(MESOS_MSVC)
    // TODO(aclemmer): doesn't work on MSVC
    throw 99;
#else /* MESOS_MSVC */
    bool occupied = true;
    synchronized (mutex) {
      occupied = waiters > 0 ? true : false;
    }
    return !occupied;
#endif /* MESOS_MSVC */
  }
};

#endif // __GATE_HPP__
