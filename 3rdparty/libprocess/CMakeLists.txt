###############################################################
#
# Copyright 2015 Microsoft Corporation
#
# Licensed under the Apache License, Version 2.0 (the "License"); you
# may not use this file except in compliance with the License.  You may
# obtain a copy of the License at
#
#    http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied
# See the License for the specific language governing permissions and
# limitations under the License.
#
##############################################################

# BUILD THE PROCESS LIBRARY
###########################
include (ExternalProject)

function (set_patch_cmd_var patch_filename patch_cmd_varname)
	# CMake does not appear to have a good way for macro functions to return
	# values, so here we assign the patch command to a variable in the
	# PARENT_SCOPE. (The name of the variable to assign to in the parent scope is
	# passed in as a parameter to the macro function.)
	set (${patch_cmd_varname}
		test ! -e ${PROCESS_3RD_SRC}/${patch_filename} || patch -p1 < ${PROCESS_3RD_SRC}/${patch_filename}
		PARENT_SCOPE)
endfunction ()

# THIRD-PARTY LIBRARIES. Tell the build system how to pull in and build third-
# party libraries at compile time, using the ExternalProject_Add macro.
###############################################################################
ExternalProject_Add(
	${BOOST_TARGET}
	PREFIX            ${BOOST_ROOT}
	CONFIGURE_COMMAND ""
	BUILD_COMMAND     ""
	INSTALL_COMMAND   ""
	URL https://github.com/apache/mesos/raw/master/3rdparty/libprocess/3rdparty/boost-1.53.0.tar.gz
	)

if (NOT WIN32)
	set (GLOG_CONFIG_CMD  ${GLOG_SRC}/../configure --prefix=${GLOG_LIB})
	set (GLOG_BUILD_CMD   make)
	set (GLOG_INSTALL_CMD make install)
	# Patch glog to deal with a problem that appears when compiling on clang
	# under the C++11 standard. cf. MESOS-860, MESOS-966.
	set_patch_cmd_var (glog-0.3.3.patch GLOG_PATCH_CMD)
endif (NOT WIN32)

ExternalProject_Add(
	${GLOG_TARGET}
	PREFIX            ${GLOG_ROOT}
	# Quotes are important in the following commands. Building glog on WIN32
	# must be done in Visual Studio, and in that case, these commands must be
	# nops. But if you take out the quote marks, CMake will treat this as an
	# empty build command, and will attempt to build glog as a CMake project.
	PATCH_COMMAND     "${GLOG_PATCH_CMD}"
	CONFIGURE_COMMAND "${GLOG_CONFIG_CMD}"
	BUILD_COMMAND     "${GLOG_BUILD_CMD}"
	INSTALL_COMMAND   "${GLOG_INSTALL_CMD}"
	URL https://google-glog.googlecode.com/files/glog-0.3.3.tar.gz
	)

ExternalProject_Add(
	${PICOJSON_TARGET}
	PREFIX            ${PICOJSON_ROOT}
	DOWNLOAD_NAME     ${PICOJSON_TARGET}.tar.gz
	CONFIGURE_COMMAND ""
	BUILD_COMMAND     ""
	INSTALL_COMMAND   ""
	URL  https://github.com/kazuho/picojson/tarball/4f93734ade33ea0f5e5b4de35fc6b2c324a8dca6
	)

if (NOT WIN32)
	set (RY_BUILD_CMD   make -C ${RY_HTTP_SRC})
	set (RY_INSTALL_CMD ar -rv libhttp_parser.a ${RY_HTTP_SRC}/http_parser_g.o)
endif (NOT WIN32)

ExternalProject_Add(
	${RY_HTTP_TARGET}
	PREFIX            ${RY_HTTP_ROOT}
	CONFIGURE_COMMAND ""
	BUILD_COMMAND     "${RY_BUILD_CMD}"
	INSTALL_COMMAND   "${RY_INSTALL_CMD}"
	# TODO(aclemmer): this depends on the github mirror, we should remove this when possible
	URL  https://github.com/apache/mesos/raw/master/3rdparty/libprocess/3rdparty/ry-http-parser-1c3624a.tar.gz
	)

if (NOT WIN32)
	set (LIBEV_CONFIG_CMD ${LIBEV_SRC}/configure)
	set (LIBEV_BUILD_CMD  make)
	# Patch libev to keep it from reaping child processes
	set_patch_cmd_var (libev-4.15.patch LIBEV_PATCH_CMD)
endif (NOT WIN32)

ExternalProject_Add(
	${LIBEV_TARGET}
	PREFIX            ${LIBEV_ROOT}
	PATCH_COMMAND     "${LIBEV_PATCH_CMD}"
	CONFIGURE_COMMAND "${LIBEV_CONFIG_CMD}"
	BUILD_COMMAND     "${LIBEV_BUILD_CMD}"
	INSTALL_COMMAND   ""
	# TODO(aclemmer): this depends on the github mirror, we should remove this when possible
	URL  https://github.com/apache/mesos/raw/master/3rdparty/libprocess/3rdparty/libev-4.15.tar.gz
	)

# SOURCE FILES FOR THE PROCESS LIBRARY
######################################
if (WIN32)
	set (PROCESS_SRC
		${PROCESS_SRC}
		src/clock.cpp
		src/config.hpp
		src/decoder.hpp
		src/encoder.hpp
		src/event_loop.hpp
		src/gate.hpp
		src/help.cpp
		#src/http.cpp            # Can't compile until VS 2015 official
		#src/io.cpp              # Can't compile until VS 2015 official
		#src/latch.cpp           # Can't compile until VS 2015 official
		src/logging.cpp
		#src/metrics/metrics.cpp # Can't compile until VS 2015 official
		src/pid.cpp
		#src/poll_socket.cpp     # Can't compile until VS 2015 official
		src/poll_socket.hpp
		#src/profiler.cpp        # Can't compile until VS 2015 official
		#src/process.cpp
		src/process_reference.hpp
		#src/reap.cpp
		#src/socket.cpp
		#src/subprocess.cpp
		src/timeseries.cpp
		)
else(WIN32)
	set (PROCESS_SRC
		${PROCESS_SRC}
		src/clock.cpp
		src/config.hpp
		src/decoder.hpp
		src/encoder.hpp
		src/event_loop.hpp
		src/gate.hpp
		src/help.cpp
		src/http.cpp
		src/io.cpp
		src/latch.cpp
		src/logging.cpp
		src/metrics/metrics.cpp
		src/pid.cpp
		src/poll_socket.cpp
		src/poll_socket.hpp
		src/profiler.cpp
		src/process.cpp
		src/process_reference.hpp
		src/reap.cpp
		src/socket.cpp
		src/subprocess.cpp
		src/timeseries.cpp
		)
endif(WIN32)

if (ENABLE_LIBEVENT)
	set(PROCESS_SRC
		${PROCESS_SRC}
		src/libevent.hpp
		src/libevent.cpp
		src/libevent_poll.cpp)
else (ENABLE_LIBEVENT)
	set (PROCESS_SRC
		${PROCESS_SRC}
		src/libev.hpp
		#src/libev.cpp
		#src/libev_poll.cpp
		)
endif (ENABLE_LIBEVENT)

# INCLUDE DIRECTIVES FOR PROCESS LIBRARY (generates, e.g., -I/path/to/thing
# on Linux)
###############################################################################
include_directories (${PROCESS_INCLUDE_DIRS})

# THE PROCESS LIBRARY (generates, e.g., libprocess.so, etc., on Linux)
###############################################################################
add_library (process ${PROCESS_SRC})

# TESTING INFRASTRUCTURE
########################
if (NOT WIN32)
	set (GMOCK_CONFIG_CMD  ${GMOCK_SRC}/configure --prefix=${GMOCK_LIB})
	set (GMOCK_BUILD_CMD   make)
endif (NOT WIN32)

ExternalProject_Add(
	${GMOCK_TARGET}
	PREFIX            ${GMOCK_ROOT}
	CONFIGURE_COMMAND "${GMOCK_CONFIG_CMD}"
	BUILD_COMMAND     "${GMOCK_BUILD_CMD}"
	INSTALL_COMMAND   ""
	# TODO(aclemmer): this depends on the github mirror, we should remove this when possible
	URL https://github.com/apache/mesos/raw/master/3rdparty/libprocess/3rdparty/gmock-1.6.0.tar.gz
	)

set (TESTS_SRC
	${TESTS_SRC}
	src/tests/decoder_tests.cpp
	src/tests/encoder_tests.cpp
	src/tests/http_tests.cpp
	src/tests/io_tests.cpp
	src/tests/limiter_tests.cpp
	src/tests/main.cpp
	src/tests/mutex_tests.cpp
	src/tests/metrics_tests.cpp
	src/tests/owned_tests.cpp
	src/tests/process_tests.cpp
	src/tests/queue_tests.cpp
	src/tests/reap_tests.cpp
	src/tests/sequence_tests.cpp
	src/tests/shared_tests.cpp
	src/tests/statistics_tests.cpp
	src/tests/subprocess_tests.cpp
	src/tests/system_tests.cpp
	src/tests/timeseries_tests.cpp
	src/tests/time_tests.cpp
	)

include_directories (
	${GMOCK_SRC}/include
	${GTEST_SRC}/include
	src
	)

link_directories (
	${PROCESS_LIB_DIRS}
	${GMOCK_BIN}/lib/.libs
	${GMOCK_BIN}/gtest/lib/.libs
	)

#add_executable (tests ${TESTS_SRC})

#target_link_libraries (tests ${PROCESS_LIBS} gmock gtest)
