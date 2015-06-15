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
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and 
# limitations under the License. 
#
###############################################################

# Common variables and paths used to set up the process library and the thrid
# party dependencies.
#
# Unlike autoconf, which makes linking to third party dependencies as simple as
# pointing at the underlying makefile, CMake seems to require that you build
# the dependencies and then specifically link a binary to them. So these are
# useful throughout the project, anywhere the third party dependencies are
# linked to.

set(PROCESS_3RD_SRC ${CMAKE_SOURCE_DIR}/3rdparty/libprocess/3rdparty/)
set(PROCESS_3RD_BIN ${CMAKE_BINARY_DIR}/3rdparty/libprocess/3rdparty/)

# VERSIONS OF OUR THIRD-PARTY LIBS
##################################
set(BOOST_VERSION          1.53.0)
set(GLOG_VERSION           0.3.3)
set(GMOCK_VERSION          1.6.0)
set(GPERFTOOLS_VERSION     2.0)
set(LIBEV_VERSION          4.15)
set(PROTOBUF_VERSION       2.5.0)
set(RY_HTTP_PARSER_VERSION 1c3624a)
set(PICOJSON_VERSION       4f93734)

# DEFINITIONS OF DIRECTORIES FOR THIRD-PARTY LIBS
#################################################
set(STOUT        ${PROCESS_3RD_SRC}/stout)

set(BOOST_TARGET boost-${BOOST_VERSION})
set(BOOST_ROOT   ${PROCESS_3RD_BIN}/${BOOST_TARGET})
set(BOOST_SRC    ${BOOST_ROOT}/src/${BOOST_TARGET})

set(GLOG_TARGET glog-${GLOG_VERSION})
set(GLOG_ROOT   ${PROCESS_3RD_BIN}/${GLOG_TARGET})
set(GLOG_SRC    ${GLOG_ROOT}/src/${GLOG_TARGET}/src)
set(GLOG_BIN    ${GLOG_ROOT}/src/${GLOG_TARGET}-build)
set(GLOG_LIB    ${GLOG_ROOT}/src/${GLOG_TARGET}-lib/lib)

set(PICOJSON_TARGET picojson-${PICOJSON_VERSION})
set(PICOJSON_ROOT   ${PROCESS_3RD_BIN}/${PICOJSON_TARGET})
set(PICOJSON_SRC    ${PICOJSON_ROOT}/src/${PICOJSON_TARGET})

set(RY_HTTP_TARGET http-parser-${RY_HTTP_PARSER_VERSION})
set(RY_HTTP_ROOT   ${PROCESS_3RD_BIN}/${RY_HTTP_TARGET})
set(RY_HTTP_SRC    ${RY_HTTP_ROOT}/src/${RY_HTTP_TARGET})
set(RY_HTTP_BIN    ${RY_HTTP_ROOT}/src/${RY_HTTP_TARGET}-build)

set(LIBEV_TARGET libev-${LIBEV_VERSION})
set(LIBEV_ROOT   ${PROCESS_3RD_BIN}/${LIBEV_TARGET})
set(LIBEV_SRC    ${LIBEV_ROOT}/src/${LIBEV_TARGET})
set(LIBEV_BIN    ${LIBEV_ROOT}/src/${LIBEV_TARGET}-build)

# Directory variables for third-party libs used by TEST INFRASTRUCTURE
set(GMOCK_TARGET gmock-${GMOCK_VERSION})
set(GMOCK_ROOT   ${PROCESS_3RD_BIN}/${GMOCK_TARGET})
set(GMOCK_SRC    ${GMOCK_ROOT}/src/${GMOCK_TARGET})
set(GMOCK_BIN    ${GMOCK_ROOT}/src/${GMOCK_TARGET}-build)
set(GMOCK_LIB    ${GMOCK_ROOT}/src/${GMOCK_TARGET}-lib/lib)

set(GTEST_SRC ${GMOCK_SRC}/gtest)

set(GPERFTOOLS   ${PROCESS_3RD_BIN}/gperftools-${GPERFTOOLS_VERSION})

# DEFINE THIRD-PARTY INCLUDE DIRECTORIES. Tells compiler toolchain where to get
# headers for our third party libs (e.g., -I/path/to/glog on Linux).
###############################################################################
set(PROCESS_INCLUDE_DIRS
	${PROCESS_INCLUDE_DIRS}
	${PROCESS_3RD_SRC}/../include
	${STOUT}/include
	${BOOST_SRC}
	${LIBEV_SRC}
	${PICOJSON_SRC}
	)

# TODO(aclemmer): Add ability to use system glog
if (WIN32)
	set (PROCESS_INCLUDE_DIRS ${PROCESS_INCLUDE_DIRS} ${GLOG_SRC}/windows)
else (WIN32)
	set (PROCESS_INCLUDE_DIRS ${PROCESS_INCLUDE_DIRS} ${GLOG_LIB}/include)
endif (WIN32)

# TODO(aclemmer): Add ability to use system HTTP parser
set(PROCESS_INCLUDE_DIRS ${PROCESS_INCLUDE_DIRS} ${RY_HTTP_SRC})

# TODO(aclemmer): Add ability to use system libev/libevent

if (HAS_GPERFTOOLS)
	set (PROCESS_INCLUDE_DIRS ${PROCESS_INCLUDE_DIRS} ${GPERFTOOLS}/src)
endif (HAS_GPERFTOOLS)

# DEFINE THIRD-PARTY LIB INSTALL DIRECTORIES. Used to tell the compiler
# toolchain where to find our third party libs (e.g., -L/path/to/glog on
# Linux).
###############################################################################
# TODO(aclemmer): make this work for system versions of these libraries, too
set(PROCESS_LIB_DIRS
	${PROCESS_LIB_DIRS}
	${GLOG_LIB}/lib
	${LIBEV_BIN}/.libs
	${RY_HTTP_BIN}
	)

# DEFINE THIRD-PARTY LIBS. Used to generate flags that the linker uses to
# include our third-party libs (e.g., -lglog on Linux).
###############################################################################
find_package(Threads REQUIRED)

set(PROCESS_LIBS
	${PROCESS_LIBS}
	process
	glog
	ev
	http_parser
	${CMAKE_THREAD_LIBS_INIT}
	)

if (NOT WIN32)
	find_package (ZLIB REQUIRED)
	find_library (LIBRT_LIBRARIES rt REQUIRED)
	
	set(PROCESS_LIBS
		${PROCESS_LIBS}
		${ZLIB_LIBRARIES}
		${LIBRT_LIBRARIES}
		)
endif (NOT WIN32)