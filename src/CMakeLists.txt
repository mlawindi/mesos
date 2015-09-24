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

# COMPILE PROTOBUF STRUCTS REQUIRED TO BUILD MESOS.
###################################################
# Convenience variables for `protoc`.
set(TO_INCLUDE_DIR
  -I${MESOS_PUBLIC_INCLUDE_DIR}
  -I${MESOS_SRC_DIR}
  --cpp_out=${MESOS_BIN_INCLUDE_DIR})

set(TO_SRC_DIR
  -I${MESOS_PUBLIC_INCLUDE_DIR}
  -I${MESOS_SRC_DIR}
  --cpp_out=${MESOS_BIN_SRC_DIR})

# Define variables for the protobuf structs we will compile.
include(MesosProtobuf)

# Build the protobuf structs.
ADD_CUSTOM_COMMAND(
  OUTPUT ${MESOS_PROTO_CC} ${MESOS_PROTO_H}
  COMMAND ${PROTOC} ${TO_INCLUDE_DIR} ${MESOS_PROTO}
  DEPENDS make_bin_include_dir
  WORKING_DIRECTORY ${MESOS_BIN})

ADD_CUSTOM_COMMAND(
  OUTPUT ${V1_MESOS_PROTO_CC} ${V1_MESOS_PROTO_H}
  COMMAND ${PROTOC} ${TO_INCLUDE_DIR} ${V1_MESOS_PROTO}
  DEPENDS make_bin_include_dir
  WORKING_DIRECTORY ${MESOS_BIN})

ADD_CUSTOM_COMMAND(
  OUTPUT ${AUTHENTICATION_PROTO_CC} ${AUTHENTICATION_PROTO_H}
  COMMAND ${PROTOC} ${TO_INCLUDE_DIR} ${AUTHENTICATION_PROTO}
  DEPENDS make_bin_include_dir
  WORKING_DIRECTORY ${MESOS_BIN})

ADD_CUSTOM_COMMAND(
  OUTPUT ${AUTHORIZATION_PROTO_CC} ${AUTHORIZATION_PROTO_H}
  COMMAND ${PROTOC} ${TO_INCLUDE_DIR} ${AUTHORIZATION_PROTO}
  DEPENDS make_bin_include_dir
  WORKING_DIRECTORY ${MESOS_BIN})

ADD_CUSTOM_COMMAND(
  OUTPUT ${CONTAINERIZER_PROTO_CC} ${CONTAINERIZER_PROTO_H}
  COMMAND ${PROTOC} ${TO_INCLUDE_DIR} ${CONTAINERIZER_PROTO}
  DEPENDS make_bin_include_dir
  WORKING_DIRECTORY ${MESOS_BIN})

ADD_CUSTOM_COMMAND(
  OUTPUT ${EXECUTOR_PROTO_CC} ${EXECUTOR_PROTO_H}
  COMMAND ${PROTOC} ${TO_INCLUDE_DIR} ${EXECUTOR_PROTO}
  DEPENDS make_bin_include_dir
  WORKING_DIRECTORY ${MESOS_BIN})

ADD_CUSTOM_COMMAND(
  OUTPUT ${V1_EXECUTOR_PROTO_CC} ${V1_EXECUTOR_PROTO_H}
  COMMAND ${PROTOC} ${TO_INCLUDE_DIR} ${V1_EXECUTOR_PROTO}
  DEPENDS make_bin_include_dir
  WORKING_DIRECTORY ${MESOS_BIN})

ADD_CUSTOM_COMMAND(
  OUTPUT ${FETCHER_PROTO_CC} ${FETCHER_PROTO_H}
  COMMAND ${PROTOC} ${TO_INCLUDE_DIR} ${FETCHER_PROTO}
  DEPENDS make_bin_include_dir
  WORKING_DIRECTORY ${MESOS_BIN})

ADD_CUSTOM_COMMAND(
  OUTPUT ${MAINTENANCE_PROTO_CC} ${MAINTENANCE_PROTO_H}
  COMMAND ${PROTOC} ${TO_INCLUDE_DIR} ${MAINTENANCE_PROTO}
  DEPENDS make_bin_include_dir
  WORKING_DIRECTORY ${MESOS_BIN})

ADD_CUSTOM_COMMAND(
  OUTPUT ${ALLOCATOR_PROTO_CC} ${ALLOCATOR_PROTO_H}
  COMMAND ${PROTOC} ${TO_INCLUDE_DIR} ${ALLOCATOR_PROTO}
  DEPENDS make_bin_include_dir
  WORKING_DIRECTORY ${MESOS_BIN})

ADD_CUSTOM_COMMAND(
  OUTPUT ${MODULE_PROTO_CC} ${MODULE_PROTO_H}
  COMMAND ${PROTOC} ${TO_INCLUDE_DIR} ${MODULE_PROTO}
  DEPENDS make_bin_include_dir
  WORKING_DIRECTORY ${MESOS_BIN})

ADD_CUSTOM_COMMAND(
  OUTPUT ${SCHEDULER_PROTO_CC} ${SCHEDULER_PROTO_H}
  COMMAND ${PROTOC} ${TO_INCLUDE_DIR} ${SCHEDULER_PROTO}
  DEPENDS make_bin_include_dir
  WORKING_DIRECTORY ${MESOS_BIN})

ADD_CUSTOM_COMMAND(
  OUTPUT ${V1_SCHEDULER_PROTO_CC} ${V1_SCHEDULER_PROTO_H}
  COMMAND ${PROTOC} ${TO_INCLUDE_DIR} ${V1_SCHEDULER_PROTO}
  DEPENDS make_bin_include_dir
  WORKING_DIRECTORY ${MESOS_BIN})

ADD_CUSTOM_COMMAND(
  OUTPUT ${ISOLATOR_PROTO_CC} ${ISOLATOR_PROTO_H}
  COMMAND ${PROTOC} ${TO_INCLUDE_DIR} ${ISOLATOR_PROTO}
  DEPENDS make_bin_include_dir
  WORKING_DIRECTORY ${MESOS_BIN})

ADD_CUSTOM_COMMAND(
  OUTPUT ${OVERSUBSCRIPTION_PROTO_CC} ${OVERSUBSCRIPTION_PROTO_H}
  COMMAND ${PROTOC} ${TO_INCLUDE_DIR} ${OVERSUBSCRIPTION_PROTO}
  DEPENDS make_bin_include_dir
  WORKING_DIRECTORY ${MESOS_BIN})




ADD_CUSTOM_COMMAND(
  OUTPUT ${MESSAGES_PROTO_CC} ${MESSAGES_PROTO_H}
  COMMAND ${PROTOC} ${TO_SRC_DIR} ${MESSAGES_PROTO}
  DEPENDS make_bin_src_dir
  WORKING_DIRECTORY ${MESOS_BIN})

ADD_CUSTOM_COMMAND(
  OUTPUT ${FLAGS_PROTO_CC} ${FLAGS_PROTO_H}
  COMMAND ${PROTOC} ${TO_SRC_DIR} ${FLAGS_PROTO}
  DEPENDS make_bin_src_dir
  WORKING_DIRECTORY ${MESOS_BIN})

# Configure Mesos files.
########################
configure_file(
  ${MESOS_PUBLIC_INCLUDE_DIR}/mesos/version.hpp.in
  ${MESOS_BIN_INCLUDE_DIR}/mesos/version.hpp
  )

# SOURCE FILES FOR THE MESOS LIBRARY.
#####################################
set(AGENT_SRC
  slave/constants.cpp
  slave/gc.cpp
  slave/flags.cpp
  slave/http.cpp
  slave/metrics.cpp
  slave/monitor.cpp
  slave/paths.cpp
  slave/qos_controller.cpp
  slave/qos_controllers/noop.cpp
  slave/resource_estimator.cpp
  slave/slave.cpp
  slave/state.cpp
  slave/status_update_manager.cpp
  slave/containerizer/containerizer.cpp
  slave/containerizer/composing.cpp
  slave/containerizer/composing.hpp
  slave/containerizer/docker.cpp
  slave/containerizer/docker.hpp
  slave/containerizer/external_containerizer.cpp
  slave/containerizer/fetcher.cpp
  slave/containerizer/isolator.cpp
  slave/containerizer/isolators/filesystem/posix.cpp
  slave/containerizer/isolators/posix/disk.cpp
  slave/containerizer/launcher.cpp
  slave/containerizer/mesos/containerizer.cpp
  slave/containerizer/mesos/launch.cpp
  slave/containerizer/provisioner/paths.cpp
  slave/containerizer/provisioner/provisioner.cpp
  slave/containerizer/provisioner/store.cpp
  slave/containerizer/provisioner/appc/paths.cpp
  slave/containerizer/provisioner/appc/spec.cpp
  slave/containerizer/provisioner/appc/store.cpp
  slave/containerizer/provisioner/backend.cpp
  slave/containerizer/provisioner/backends/copy.cpp
  slave/containerizer/provisioner/docker/registry_client.cpp
  slave/containerizer/provisioner/docker/token_manager.cpp
  slave/resource_estimators/noop.cpp
  )

set(MESOS_SRC
  ${MESOS_SRC}
  ${AGENT_SRC}
  ${MESOS_PROTOBUF_SRC}
  )

# INCLUDE DIRECTIVES FOR MESOS LIBRARY (generates, e.g., -I/path/to/thing
# on Linux).
#########################################################################
include_directories(${AGENT_INCLUDE_DIRS})

# THE MESOS LIBRARY (generates, e.g., libmesos.so, etc., on Linux).
###################################################################
add_library(${MESOS_TARGET} ${MESOS_SRC})
set_target_properties(
  ${MESOS_TARGET} PROPERTIES
  VERSION ${MESOS_PACKAGE_VERSION}
  SOVERSION ${MESOS_PACKAGE_SOVERSION}
  )
add_dependencies(${MESOS_TARGET} ${AGENT_DEPENDENCIES})
