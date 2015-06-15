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

# SYSTEM CONFIGURATION INFORMATION
##################################
string(TOUPPER "${CMAKE_SYSTEM_NAME}" OS_NAME)
string(TOUPPER "${CMAKE_SYSTEM_VERSION}" OS_VER)
string(TOUPPER "${CMAKE_SYSTEM_PROCESSOR}" SYS_ARCH)

# CMAKE CONFIGURE LOGO
######################
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

# CONFIGURE COMPILATION
####################
if (_DEBUG)
	set( CMAKE_BUILD_TYPE Debug )
endif (_DEBUG)

set_property(GLOBAL PROPERTY CXX_STANDARD 11)
set_property(GLOBAL PROPERTY CXX_STANDARD_REQUIRED true)
# TODO(aclemmer): dependency required for Windows for some reason; remove it
#set (CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-std=c++11 -fms-compatibility -fdelayed-template-parsing -fms-extensions -target x86_64-pc-windows-msvc")
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -std=c++11)

# Convenience flags to simplify Windows support in C++ source
if (MSVC)
	add_definitions(-DMESOS_MSVC)
else (MSVC)
	add_definitions(-DMESOS_MINGW)
endif (MSVC)

# Compiler constants required for third-party libs
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

# THIRD-PARTY CONFIGURATION
include(DependencyConfigure)
#include(ProtobufConfigure)