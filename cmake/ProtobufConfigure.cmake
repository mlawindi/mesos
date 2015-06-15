# PROTOBUF THINGS
find_package(Protobuf REQUIRED)
#file(GLOB_RECURSE ProtoFiles "${CMAKE_CURRENT_SOURCE_DIR}/*.proto")
set (ProtoFiles
	include/mesos/mesos.proto
	)
message(${ProtoFiles})
include_directories(${PROTOBUF_INCLUDE_DIRS})
PROTOBUF_GENERATE_CPP(ProtoSources ProtoHeaders ${ProtoFiles})
add_library(messages STATIC ${ProtoSources} ${ProtoHeaders})
target_link_libraries(messages ${PROTOBUF_LIBRARY})

file(COPY ${CMAKE_CURRENT_BINARY_DIR}/mesos.pb.h DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/messages/mesos)

PROTOBUF_GENERATE_CPP(ProtoSources ProtoHeaders include/mesos/module/module.proto)
add_library(messages2 STATIC ${ProtoSources} ${ProtoHeaders})
target_link_libraries(messages2 ${PROTOBUF_LIBRARY})
#file(COPY ${CMAKE_CURRENT_BINARY_DIR}/module.pb.h DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/messages/mesos)
