cmake_minimum_required(VERSION 3.0)
project(kytea-grpc)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -g")

find_package(Protobuf REQUIRED)
find_package(GRPC REQUIRED)
find_package(Kytea REQUIRED)

set(ProtoFiles protobuf/kytea.proto protobuf/kytea-svc.proto)

PROTOBUF_GENERATE_CPP(PROTO_SRCS GRPC_SRCS ${ProtoFiles})
PROTOBUF_GENERATE_GRPC_CPP(GRPC_SRCS GRPC_HDRS protobuf/kytea-svc.proto)

include_directories(${CMAKE_CURRENT_BINARY_DIR} ${Protobuf_INCLUDE_DIR} ${GRPC_INCLUDE_DIR} ${KYTEA_INCLUDE_DIR})

set(SOURCES
    src/kytea-grpc.cpp
    ${PROTO_SRCS} ${PROTO_HDRS}
    ${GRPC_SRCS} ${GRPC_HDRS}
)

add_executable(kytea-grpc ${SOURCES})

target_link_libraries(kytea-grpc ${Protobuf_LIBRARIES} ${GRPC_LIBRARIES} ${KYTEA_LIBRARIES})