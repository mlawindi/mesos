#ifndef __PROCESS_REAP_HPP__
#define __PROCESS_REAP_HPP__

#include <sys/types.h>

#include <process/future.hpp>

#include <stout/option.hpp>

namespace process {

// The upper bound for the poll interval in the reaper.
Duration MAX_REAP_INTERVAL();

#if defined(MESOS_MSVC)
#else /* MESOS_MSVC */
// Returns the exit status of the specified process if and only if
// the process is a direct child and it has not already been reaped.
// Otherwise, returns None once the process has been reaped elsewhere
// (or does not exist, which is indistinguishable from being reaped
// elsewhere). This will never discard the returned future.
Future<Option<int> > reap(pid_t pid);
#endif /* MESOS_MSVC */

} // namespace process {

#endif // __PROCESS_REAP_HPP__
