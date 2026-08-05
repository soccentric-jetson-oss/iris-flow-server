cmake_minimum_required(VERSION 3.20)
project(iris-flow-server VERSION 0.1.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Protobuf REQUIRED)
find_package(gRPC REQUIRED)
find_package(Threads REQUIRED)
include(cmake/CompilerWarnings.cmake)

set(PROTO_SRC ${CMAKE_CURRENT_SOURCE_DIR}/proto/iris_flow.proto)
set(PROTO_GEN_DIR ${CMAKE_CURRENT_BINARY_DIR}/generated)
file(MAKE_DIRECTORY ${PROTO_GEN_DIR})

add_custom_command(
    OUTPUT ${PROTO_GEN_DIR}/iris_flow.pb.cc ${PROTO_GEN_DIR}/iris_flow.grpc.pb.cc
    COMMAND protoc --proto_path ${CMAKE_CURRENT_SOURCE_DIR}/proto
        --cpp_out ${PROTO_GEN_DIR} --grpc_out ${PROTO_GEN_DIR}
        --plugin=protoc-gen-grpc=$<TARGET_FILE:gRPC::grpc_cpp_plugin>
        ${PROTO_SRC}
    DEPENDS ${PROTO_SRC}
)

add_library(iris_flow_proto OBJECT ${PROTO_GEN_DIR}/iris_flow.pb.cc ${PROTO_GEN_DIR}/iris_flow.grpc.pb.cc)
target_include_directories(iris_flow_proto PUBLIC ${PROTO_GEN_DIR})
target_link_libraries(iris_flow_proto PUBLIC protobuf::libprotobuf gRPC::grpc++)
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(iris_flow_proto PRIVATE -Wno-sign-conversion)
endif()

add_executable(iris-flow-server src/main.cpp)
target_include_directories(iris-flow-server PRIVATE src ${PROTO_GEN_DIR})
target_link_libraries(iris-flow-server PRIVATE iris_flow_proto protobuf::libprotobuf gRPC::grpc++ Threads::Threads)
target_strict_warnings(iris-flow-server)

# Tests
option(BUILD_TESTS "Build tests" ON)
if(BUILD_TESTS)
    enable_testing()
    include(FetchContent)
    FetchContent_Declare(Catch2 GIT_REPOSITORY https://github.com/catchorg/Catch2.git GIT_TAG v3.4.0)
    FetchContent_MakeAvailable(Catch2)
    add_executable(iris-flow-server-test test/test_service.cpp)
    target_include_directories(iris-flow-server-test PRIVATE src ${PROTO_GEN_DIR})
    target_link_libraries(iris-flow-server-test PRIVATE iris_flow_proto Catch2::Catch2WithMain)
    add_test(NAME iris-flow-server-test COMMAND iris-flow-server-test)
endif()
