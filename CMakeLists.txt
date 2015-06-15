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

# THE MESOS PROJECT
###################
cmake_minimum_required(VERSION 3.2)

project(Mesos)
set(MESOS_MAJOR_VERSION 0)
set(MESOS_MINOR_VERSION 22.1)
set(VERSION "${MESOS_MAJOR_VERSION}.${MESOS_MINOR_VERSION}")
set(PACKAGE_VERSION ${VERSION})

# MESOS BUILD CONFIGURATION OPTIONS
###################################
option(_DEBUG "Set default build configuration to debug" OFF)
option(VERBOSE "Enable verbose CMake statements and compilation output" FALSE)
set(CMAKE_VERBOSE_MAKEFILE ${VERBOSE})

# CMAKE MODULE SETUP
####################
list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)
include(MesosConfigure)

# CONFIGURATION THINGS
configure_file(
	${CMAKE_CURRENT_SOURCE_DIR}/include/mesos/version.hpp.in
	${CMAKE_CURRENT_SOURCE_DIR}/include/mesos/version.hpp @ONLY)

# SUBDIRECTORIES
################
add_subdirectory(3rdparty)
#add_subdirectory(src)

# INSTALLATION TARGET
#####################
# TODO(aclemmer): Finish this
#install(DIRECTORY include/mesos DESTINATION