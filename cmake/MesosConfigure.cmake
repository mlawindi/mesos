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
include(CheckCXXCompilerFlag)

# SYSTEM CONFIGURATION INFORMATION.
###################################
string(TOUPPER "${CMAKE_SYSTEM_NAME}"      OS_NAME)
string(TOUPPER "${CMAKE_SYSTEM_VERSION}"   OS_VER)
string(TOUPPER "${CMAKE_SYSTEM_PROCESSOR}" SYS_ARCH)

# CMAKE CONFIGURE LOGO.
#######################
message(STATUS "************************************************************")
message(STATUS "********* Beginning Mesos CMake configuration step *********")
message(STATUS "************************************************************")
message(STATUS "INSTALLATION PREFIX: ${CMAKE_INSTALL_PREFIX}")
message(STATUS "MACHINE SPECS:")
message(STATUS "    Hostname: ${HOSTNAME}")
message(STATUS "    OS:       ${OS_NAME}(${OS_VER})")
message(STATUS "    Arch:     ${SYS_ARCH}")
message(STATUS "    BitMode:  ${BIT_MODE}")
message(STATUS "    BuildID:  ${BUILDID}")
message(STATUS "************************************************************")

# SET UP TESTING INFRASTRUCTURE.
################################
enable_testing()

# CONFIGURE COMPILATION.
########################
string(COMPARE EQUAL ${CMAKE_SYSTEM_NAME} "Linux" LINUX)

if (_DEBUG)
  set(CMAKE_BUILD_TYPE Debug)
endif (_DEBUG)

# Make sure C++ 11 features we need are supported. This is split into two
# cases: Windows and "other platforms".
#   * For "other platforms", we simply check if the C++11 flags work
#   * For Windows, it looks like (1) C++11 is enabled by default on MSVC 1900 or
#     later, and (2) C++11 is totally broken for 1800 or earlier (i.e., Mesos
#     will not compile on MSVC pre-1900). So, when in Windows, we just check the
#     MSVC version, and don't try to check or pass in C++11 flags at all.
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
if (WIN32)
  # Windows case first.

  # We don't support compilation against mingw headers (which, e.g., Clang on
  # Windows does at this point), because this is likely to cost us more effort
  # to support than it will be worth at least in the short term.
  if (NOT CMAKE_CXX_COMPILER_ID MATCHES MSVC)
    message(
      WARNING
      "Mesos does not support compiling on Windows with "
      "${CMAKE_CXX_COMPILER_ID}. Please use MSVC.")
  endif (NOT CMAKE_CXX_COMPILER_ID MATCHES MSVC)

  # MSVC 1900 supports C++11; earliser versions don't. So, warn if you try to
  # use anything else.
  if (${MSVC_VERSION} LESS 1900)
    message(
      WARNING
      "Mesos does not support compiling on MSVC versions earlier than 1900. "
      "Please use MSVC 1900 (included with Visual Studio 2015 or later).")
  endif (${MSVC_VERSION} LESS 1900)

  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
  set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
elseif (COMPILER_SUPPORTS_CXX11)
  # Finally, on non-Windows platforms, we must check that the current compiler
  # supports C++11.
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
else (WIN32)
  message(
    FATAL_ERROR
    "The compiler ${CMAKE_CXX_COMPILER} does not support the `-std=c++11` "
    "flag. Please use a different C++ compiler.")
endif (WIN32)

# Convenience flags to simplify Windows support in C++ source.
if (MSVC)
  add_definitions(-DMESOS_MSVC)
endif (MSVC)

# Compiler constants required for third-party libs.
if (WIN32)
  # Windows-specific workaround for a glog issue documented here[1].
  # Basically, Windows.h and glog/logging.h both define ERROR. Since we don't
  # need the Windows ERROR, we can use this flag to avoid defining it at all.
  # Unlike the other fix (defining GLOG_NO_ABBREVIATED_SEVERITIES), this fix
  # is guaranteed to require no changes to the original Mesos code. See also
  # the note in the code itself[2].
  #
  # [1] https://google-glog.googlecode.com/svn/trunk/doc/glog.html#windows
  # [2] https://code.google.com/p/google-glog/source/browse/trunk/src/windows/glog/logging.h?r=113
  add_definitions(-DNOGDI)
  add_definitions(-DNOMINMAX)
endif (WIN32)

# THIRD-PARTY CONFIGURATION.
############################
# NOTE: The third-party configuration variables exported here are used
# throughout the project, so it's important that this config script goes here.
include(Process3rdpartyConfigure)

# Generate a batch script that will build Mesos. Any project referencing Mesos
# can then build it by calling this script.
if (WIN32)
  VS_BUILD_CMD(
      MESOS
      ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.sln
      ${CMAKE_BUILD_TYPE}
      "")

  string(REPLACE ";" " " MESOS_BUILD_CMD "${MESOS_BUILD_CMD}")
  file(WRITE ${CMAKE_BINARY_DIR}/make.bat ${MESOS_BUILD_CMD})
endif (WIN32)

# Add preprocessor definitions required to build third-party libraries.
#######################################################################
add_definitions(-DPKGLIBEXECDIR="WARNINGDONOTUSEME")

# Enable the INT64 support for PicoJSON.
add_definitions(-DPICOJSON_USE_INT64)
# NOTE: PicoJson requires __STDC_FORMAT_MACROS to be defined before importing
# 'inttypes.h'.  Since other libraries may also import this header, it must
# be globally defined so that PicoJson has access to the macros, regardless
# of the order of inclusion.
add_definitions(-D__STDC_FORMAT_MACROS)

# CONFIGURE MESOS.
##################
set(MESOS_TARGET mesos-${MESOS_PACKAGE_VERSION})

# DEFINE DIRECTORY STRUCTURE FOR THIRD-PARTY LIBS.
##################################################
set(MESOS_SRC_DIR     ${CMAKE_SOURCE_DIR}/src)
set(MESOS_BIN         ${CMAKE_BINARY_DIR})
set(MESOS_BIN_SRC_DIR ${MESOS_BIN}/src)

# Convenience variables for include directories of third-party dependencies.
set(MESOS_PUBLIC_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)
set(MESOS_BIN_INCLUDE_DIR    ${CMAKE_BINARY_DIR}/include)

# Make directories that generated Mesos code goes into.
#######################################################
add_custom_target(
  make_bin_include_dir ALL
  COMMAND ${CMAKE_COMMAND} -E make_directory ${MESOS_BIN_INCLUDE_DIR})

add_custom_target(
  make_bin_src_dir ALL
  COMMAND ${CMAKE_COMMAND} -E make_directory ${MESOS_BIN_SRC_DIR})

# Define process library dependencies. Tells the process library build targets
# download/configure/build all third-party libraries before attempting to build.
################################################################################
set(MESOS_DEPENDENCIES
  ${MESOS_DEPENDENCIES}
  ${PROCESS_TARGET}
  ${BOOST_TARGET}
  ${GLOG_TARGET}
  ${PICOJSON_TARGET}
  make_bin_include_dir
  make_bin_src_dir
  )

# Define third-party include directories. Tells compiler toolchain where to get
# headers for our third party libs (e.g., -I/path/to/glog on Linux).
###############################################################################
set(MESOS_INCLUDE_DIRS
  ${MESOS_INCLUDE_DIRS}
  ${MESOS_PUBLIC_INCLUDE_DIR}
  # Protobuf headers that depend on mesos.pb.h need this.
  ${MESOS_PUBLIC_INCLUDE_DIR}/mesos
  # Contains (e.g.) compiled *.pb.h files.
  ${MESOS_BIN_INCLUDE_DIR}
  ${MESOS_BIN_SRC_DIR}
  ${MESOS_SRC_DIR}

  ${PROCESS_INCLUDE_DIR}
  ${STOUT_INCLUDE_DIR}
  ${BOOST_INCLUDE_DIR}
  ${GLOG_INCLUDE_DIR}
  ${PICOJSON_INCLUDE_DIR}
  ${PROTOBUF_INCLUDE_DIR}
  )

# Define third-party lib install directories. Used to tell the compiler
# toolchain where to find our third party libs (e.g., -L/path/to/glog on
# Linux).
########################################################################
set(MESOS_LIB_DIRS
  ${PROCESS_LIB_DIRS}
  ${GLOG_LIB_DIR}
  )

# Define third-party libs. Used to generate flags that the linker uses to
# include our third-party libs (e.g., -lglog on Linux).
#########################################################################
set(MESOS_LIBS
  ${PROCESS_LIBS}
  ${GLOG_LFLAG}
  )
