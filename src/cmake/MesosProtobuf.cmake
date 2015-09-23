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

set(MESOS_PROTO
  ${MESOS_PUBLIC_INCLUDE_DIR}/mesos/mesos.proto)
set(MESOS_PROTO_CC
  ${MESOS_BIN_INCLUDE_DIR}/mesos/mesos.pb.cc)
set(MESOS_PROTO_H
  ${MESOS_BIN_INCLUDE_DIR}/mesos/mesos.pb.h)

set(V1_MESOS_PROTO
  ${MESOS_PUBLIC_INCLUDE_DIR}/mesos/v1/mesos.proto)
set(V1_MESOS_PROTO_CC
  ${MESOS_BIN_INCLUDE_DIR}/mesos/v1/mesos.pb.cc)
set(V1_MESOS_PROTO_H
  ${MESOS_BIN_INCLUDE_DIR}/mesos/v1/mesos.pb.h)

set(AUTHENTICATION_PROTO
  ${MESOS_PUBLIC_INCLUDE_DIR}/mesos/authentication/authentication.proto)
set(AUTHENTICATION_PROTO_CC
  ${MESOS_BIN_INCLUDE_DIR}/mesos/authentication/authentication.pb.cc)
set(AUTHENTICATION_PROTO_H
  ${MESOS_BIN_INCLUDE_DIR}/mesos/authentication/authentication.pb.h)

set(AUTHORIZATION_PROTO
  ${MESOS_PUBLIC_INCLUDE_DIR}/mesos/authorizer/authorizer.proto)
set(AUTHORIZATION_PROTO_CC
  ${MESOS_BIN_INCLUDE_DIR}/mesos/authorizer/authorizer.pb.cc)
set(AUTHORIZATION_PROTO_H
  ${MESOS_BIN_INCLUDE_DIR}/mesos/authorizer/authorizer.pb.h)

set(CONTAINERIZER_PROTO
  ${MESOS_PUBLIC_INCLUDE_DIR}/mesos/containerizer/containerizer.proto)
set(CONTAINERIZER_PROTO_CC
  ${MESOS_BIN_INCLUDE_DIR}/mesos/containerizer/containerizer.pb.cc)
set(CONTAINERIZER_PROTO_H
  ${MESOS_BIN_INCLUDE_DIR}/mesos/containerizer/containerizer.pb.h)

set(EXECUTOR_PROTO
  ${MESOS_PUBLIC_INCLUDE_DIR}/mesos/executor/executor.proto)
set(EXECUTOR_PROTO_CC
  ${MESOS_BIN_INCLUDE_DIR}/mesos/executor/executor.pb.cc)
set(EXECUTOR_PROTO_H
  ${MESOS_BIN_INCLUDE_DIR}/mesos/executor/executor.pb.h)

set(V1_EXECUTOR_PROTO
  ${MESOS_PUBLIC_INCLUDE_DIR}/mesos/v1/executor/executor.proto)
set(V1_EXECUTOR_PROTO_CC
  ${MESOS_BIN_INCLUDE_DIR}/mesos/v1/executor/executor.pb.cc)
set(V1_EXECUTOR_PROTO_H
  ${MESOS_BIN_INCLUDE_DIR}/mesos/v1/executor/executor.pb.h)

set(FETCHER_PROTO
  ${MESOS_PUBLIC_INCLUDE_DIR}/mesos/fetcher/fetcher.proto)
set(FETCHER_PROTO_CC
  ${MESOS_BIN_INCLUDE_DIR}/mesos/fetcher/fetcher.pb.cc)
set(FETCHER_PROTO_H
  ${MESOS_BIN_INCLUDE_DIR}/mesos/fetcher/fetcher.pb.h)

set(MAINTENANCE_PROTO
  ${MESOS_PUBLIC_INCLUDE_DIR}/mesos/maintenance/maintenance.proto)
set(MAINTENANCE_PROTO_CC
  ${MESOS_BIN_INCLUDE_DIR}/mesos/maintenance/maintenance.pb.cc)
set(MAINTENANCE_PROTO_H
  ${MESOS_BIN_INCLUDE_DIR}/mesos/maintenance/maintenance.pb.h)

set(ALLOCATOR_PROTO
  ${MESOS_PUBLIC_INCLUDE_DIR}/mesos/master/allocator.proto)
set(ALLOCATOR_PROTO_CC
  ${MESOS_BIN_INCLUDE_DIR}/mesos/master/allocator.pb.cc)
set(ALLOCATOR_PROTO_H
  ${MESOS_BIN_INCLUDE_DIR}/mesos/master/allocator.pb.h)

set(MODULE_PROTO
  ${MESOS_PUBLIC_INCLUDE_DIR}/mesos/module/module.proto)
set(MODULE_PROTO_CC
  ${MESOS_BIN_INCLUDE_DIR}/mesos/module/module.pb.cc)
set(MODULE_PROTO_H
  ${MESOS_BIN_INCLUDE_DIR}/mesos/module/module.pb.h)

set(SCHEDULER_PROTO
  ${MESOS_PUBLIC_INCLUDE_DIR}/mesos/scheduler/scheduler.proto)
set(SCHEDULER_PROTO_CC
  ${MESOS_BIN_INCLUDE_DIR}/mesos/scheduler/scheduler.pb.cc)
set(SCHEDULER_PROTO_H
  ${MESOS_BIN_INCLUDE_DIR}/mesos/scheduler/scheduler.pb.h)

set(V1_SCHEDULER_PROTO
  ${MESOS_PUBLIC_INCLUDE_DIR}/mesos/v1/scheduler/scheduler.proto)
set(V1_SCHEDULER_PROTO_CC
  ${MESOS_BIN_INCLUDE_DIR}/mesos/v1/scheduler/scheduler.pb.cc)
set(V1_SCHEDULER_PROTO_H
  ${MESOS_BIN_INCLUDE_DIR}/mesos/v1/scheduler/scheduler.pb.h)

set(ISOLATOR_PROTO
  ${MESOS_PUBLIC_INCLUDE_DIR}/mesos/slave/isolator.proto)
set(ISOLATOR_PROTO_CC
  ${MESOS_BIN_INCLUDE_DIR}/mesos/slave/isolator.pb.cc)
set(ISOLATOR_PROTO_H
  ${MESOS_BIN_INCLUDE_DIR}/mesos/slave/isolator.pb.h)

set(OVERSUBSCRIPTION_PROTO
  ${MESOS_PUBLIC_INCLUDE_DIR}/mesos/slave/oversubscription.proto)
set(OVERSUBSCRIPTION_PROTO_CC
  ${MESOS_BIN_INCLUDE_DIR}/mesos/slave/oversubscription.pb.cc)
set(OVERSUBSCRIPTION_PROTO_H
  ${MESOS_BIN_INCLUDE_DIR}/mesos/slave/oversubscription.pb.h)

set(MESSAGES_PROTO
  ${MESOS_SRC_DIR}/messages/messages.proto)
set(MESSAGES_PROTO_CC
  ${MESOS_BIN_SRC_DIR}/messages/messages.pb.cc)
set(MESSAGES_PROTO_H
  ${MESOS_BIN_SRC_DIR}/messages/messages.pb.h)

set(FLAGS_PROTO
  ${MESOS_SRC_DIR}/messages/flags.proto)
set(FLAGS_PROTO_CC
  ${MESOS_BIN_SRC_DIR}/messages/flags.pb.cc)
set(FLAGS_PROTO_H
  ${MESOS_BIN_SRC_DIR}/messages/flags.pb.h)

set(MESOS_PROTOBUF_SRC
  ${MESOS_PROTO_CC}
  ${V1_MESOS_PROTO_CC}
  ${AUTHENTICATION_PROTO_CC}
  ${AUTHORIZATION_PROTO_CC}
  ${CONTAINERIZER_PROTO_CC}
  ${EXECUTOR_PROTO_CC}
  ${V1_EXECUTOR_PROTO_CC}
  ${FETCHER_PROTO_CC}
  ${MAINTENANCE_PROTO_CC}
  ${ALLOCATOR_PROTO_CC}
  ${MODULE_PROTO_CC}
  ${SCHEDULER_PROTO_CC}
  ${V1_SCHEDULER_PROTO_CC}
  ${MESSAGES_PROTO_CC}
  ${FLAGS_PROTO_CC}
  ${ISOLATOR_PROTO_CC}
  ${OVERSUBSCRIPTION_PROTO_CC}
  )
