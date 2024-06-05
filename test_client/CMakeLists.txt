cmake_minimum_required(VERSION 3.20)
project(client)


set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
find_package(Protobuf CONFIG REQUIRED)
find_package(gRPC CONFIG REQUIRED)
find_package(jwt-cpp CONFIG REQUIRED)
find_package(SQLite3 CONFIG REQUIRED)
find_package(yaml-cpp CONFIG REQUIRED)

add_library(protolib proto/hello.proto proto/sso.proto proto/auth.proto)
target_link_libraries(protolib gRPC::grpc++)


target_include_directories(protolib PUBLIC ${CMAKE_CURRENT_BINARY_DIR})
get_target_property(grpc_cpp_plugin_location gRPC::grpc_cpp_plugin LOCATION)
protobuf_generate(TARGET protolib LANGUAGE cpp)
protobuf_generate(TARGET protolib LANGUAGE grpc
GENERATE_EXTENSIONS .grpc.pb.h .grpc.pb.cc
PLUGIN "protoc-gen-grpc=${grpc_cpp_plugin_location}"
)


add_executable(client src/client.cpp)
target_link_libraries(client protolib  jwt-cpp::jwt-cpp SQLite::SQLite3 yaml-cpp::yaml-cpp)
