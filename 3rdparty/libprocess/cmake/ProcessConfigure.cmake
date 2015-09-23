# Licensed to the Apache Software Foundation (ASF) under one
# or more contributor license agreements.  See the NOTICE file
# distributed with this work for additional information
# regarding copyright ownership.  The ASF licenses this file
# to you under the Apache License, Version 2.0 (the
# "License"); you may not use this file except in compliance
# with the License.  You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

###############################################################
# This file exports variables needed ot link to third-party libs. These are
# used throughout the Mesos project.
#
# This includes things like:
#   * Components defining the public interface, like which headers we need in
#     order to link to libprocess.
#   * Where to look to find built libraries.
#   * Version information of the third-party libraries in use.
#
# This does not include:
#   * Where to find include headers for tests -- the rest of Mesos does not
#     need this information.
#   * Any information about how to build these libraries. That's in
#     libprocess/CMakeLists.txt
#   * Information required to build third-party libraries, wuch as what source
#     files we need to compile libprocess.
#   * Build commands actually used to compile (e.g.) libprocess.
#
# Rationale: Autoconf makes linking to third party dependencies as simple as
# pointing at the underlying makefile. In CMake, this is harder because we do
# not assume there are Makefiles at all. Thus, it is useful to export variables
# with things like which header files you need to include to link to third
# party libraries, and where in the directory tree you need to look to get the
# actual libraries.

set(PROCESS_PACKAGE_VERSION 0.0.1)
set(PROCESS_PACKAGE_SOVERSION 0)
set(PROCESS_TARGET process-${PROCESS_PACKAGE_VERSION})

# DEFINE DIRECTORY STRUCTURE FOR THIRD-PARTY LIBS.
##################################################
set(PROCESS_3RD_SRC ${CMAKE_SOURCE_DIR}/3rdparty/libprocess/3rdparty)
set(PROCESS_3RD_BIN ${CMAKE_BINARY_DIR}/3rdparty/libprocess/3rdparty)

set(STOUT ${PROCESS_3RD_SRC}/stout)

EXTERNAL("boost"       ${BOOST_VERSION}       "${PROCESS_3RD_BIN}")
EXTERNAL("picojson"    ${PICOJSON_VERSION}    "${PROCESS_3RD_BIN}")
EXTERNAL("http_parser" ${HTTP_PARSER_VERSION} "${PROCESS_3RD_BIN}")
EXTERNAL("libev"       ${LIBEV_VERSION}       "${PROCESS_3RD_BIN}")
EXTERNAL("libevent"    ${LIBEVENT_VERSION}    "${PROCESS_3RD_BIN}")
EXTERNAL("libapr"      ${LIBAPR_VERSION}      "${PROCESS_3RD_BIN}")
EXTERNAL("protobuf"    ${PROTOBUF_VERSION}    "${PROCESS_3RD_BIN}")

if (NOT WIN32)
  EXTERNAL("glog" ${GLOG_VERSION} "${PROCESS_3RD_BIN}")
elseif (WIN32)
  # Glog 0.3.3 does not compile out of the box on Windows. Therefore, we
  # require 0.3.4.
  EXTERNAL("glog" "0.3.4" "${PROCESS_3RD_BIN}")

  # NOTE: We expect cURL exists on Unix (usually pulled in with a package
  # manager), but Windows has no package manager, so we have to go get it.
  EXTERNAL("curl" ${CURL_VERSION} "${PROCESS_3RD_BIN}")
endif (NOT WIN32)

set(GLOG_LIB_ROOT ${GLOG_ROOT}-lib/lib)
set(PROTOBUF_LIB  ${PROTOBUF_ROOT}-lib/lib)

# Convenience variables for include directories of third-party dependencies.
set(PROCESS_INCLUDE_DIR     ${PROCESS_3RD_SRC}/../include)
set(STOUT_INCLUDE_DIR       ${STOUT}/include)

set(BOOST_INCLUDE_DIR       ${BOOST_ROOT})
set(GPERFTOOLS_INCLUDE_DIR  ${GPERFTOOLS}/src)
set(HTTP_PARSER_INCLUDE_DIR ${HTTP_PARSER_ROOT})
set(LIBEV_INCLUDE_DIR       ${LIBEV_ROOT})
set(PICOJSON_INCLUDE_DIR    ${PICOJSON_ROOT})
set(PROTOBUF_INCLUDE_DIR    ${PROTOBUF_LIB}/include)

if (WIN32)
  set(CURL_INCLUDE_DIR ${CURL_ROOT}/include)
  set(GLOG_INCLUDE_DIR ${GLOG_ROOT}/src/windows)
else (WIN32)
  set(GLOG_INCLUDE_DIR ${GLOG_LIB_ROOT}/include)
endif (WIN32)

# Convenience variables for `lib` directories of built third-party dependencies.
set(HTTP_PARSER_LIB_DIR ${HTTP_PARSER_ROOT}-build)
set(LIBEV_LIB_DIR       ${LIBEV_ROOT}-build/.libs)

if (WIN32)
  set(CURL_LIB_DIR     ${CURL_ROOT}/lib)
  set(GLOG_LIB_DIR     ${GLOG_ROOT}/Debug)
  set(PROTOBUF_LIB_DIR ${PROTOBUF_ROOT}/vsprojects/Debug)
else (WIN32)
  set(GLOG_LIB_DIR     ${GLOG_LIB_ROOT}/lib)
  set(PROTOBUF_LIB_DIR ${PROTOBUF_LIB}/lib)
endif (WIN32)

# Convenience variables for "lflags", the symbols we pass to CMake to generate
# things like `-L/path/to/glog` or `-lglog`.
set(HTTP_PARSER_LFLAG http_parser)
set(LIBEV_LFLAG       ev)

if (WIN32)
  # Necessary because the lib names for (e.g.) glog are generated incorrectly
  # on Windows. That is, on *nix, the glog binary should be (e.g.) libglog.so,
  # and on Windows it should be glog.lib. But on Windows, it's actually
  # libglog.lib. Hence, we have to special case it here because CMake assumes
  # the library names are generated correctly.
  set(CURL_LFLAG     libcurl_a)
  set(GLOG_LFLAG     libglog)
  set(PROTOBUF_LFLAG libprotobuf)
else (WIN32)
  set(GLOG_LFLAG     glog)
  set(PROTOBUF_LFLAG protobuf)
endif (WIN32)

# Convenience variable for `protoc`, the Protobuf compiler.
set(PROTOC ${PROTOBUF_LIB}/bin/protoc)

# Define process library dependencies. Tells the process library build targets
# download/configure/build all third-party libraries before attempting to build.
################################################################################
set(PROCESS_DEPENDENCIES
  ${PROCESS_DEPENDENCIES}
  ${BOOST_TARGET}
  ${GLOG_TARGET}
  ${PICOJSON_TARGET}
  ${HTTP_PARSER_TARGET}
  ${LIBEV_TARGET}
  ${PROTOBUF_TARGET}
  )

# Define third-party include directories. Tells compiler toolchain where to get
# headers for our third party libs (e.g., -I/path/to/glog on Linux).
###############################################################################
set(PROCESS_INCLUDE_DIRS
  ${PROCESS_INCLUDE_DIRS}
  ${PROCESS_INCLUDE_DIR}
  ${STOUT_INCLUDE_DIR}
  ${BOOST_INCLUDE_DIR}
  ${LIBEV_INCLUDE_DIR}
  ${PICOJSON_INCLUDE_DIR}
  ${GLOG_INCLUDE_DIR}
  ${HTTP_PARSER_INCLUDE_DIR}
  )

if (HAS_GPERFTOOLS)
  set(PROCESS_INCLUDE_DIRS ${PROCESS_INCLUDE_DIRS} ${GPERFTOOLS_INCLUDE_DIR})
endif (HAS_GPERFTOOLS)

# Define third-party lib install directories. Used to tell the compiler
# toolchain where to find our third party libs (e.g., -L/path/to/glog on
# Linux).
########################################################################
set(PROCESS_LIB_DIRS
  ${PROCESS_LIB_DIRS}
  ${GLOG_LIB_DIR}
  ${LIBEV_LIB_DIR}
  ${HTTP_PARSER_LIB_DIR}
  )

# Define third-party libs. Used to generate flags that the linker uses to
# include our third-party libs (e.g., -lglog on Linux).
#########################################################################
find_package(Threads REQUIRED)

set(PROCESS_LIBS
  ${PROCESS_LIBS}
  ${PROCESS_TARGET}
  ${GLOG_LFLAG}
  ${LIBEV_LFLAG}
  ${HTTP_PARSER_LFLAG}
  ${CMAKE_THREAD_LIBS_INIT}
  )

if (NOT WIN32)
  find_package(ZLIB REQUIRED)

  # TODO(hausdorff): (MESOS-3396) The `LINUX` flag comes from MesosConfigure;
  # when we port the bootstrap script to CMake, we should also copy this
  # logic into .cmake files in the Stout and Process libraries'
  # folders individually.
  if (LINUX)
    set(PROCESS_LIBS ${PROCESS_LIBS} rt)
  endif (LINUX)

  set(PROCESS_LIBS
    ${PROCESS_LIBS}
    ${ZLIB_LIBRARIES}
    )
endif (NOT WIN32)
