cmake_minimum_required(VERSION 3.27)
project(qtui)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(Protobuf CONFIG REQUIRED)
find_package(gRPC CONFIG REQUIRED)
find_package(jwt-cpp CONFIG REQUIRED)
find_package(SQLite3 CONFIG REQUIRED)
find_package(yaml-cpp CONFIG REQUIRED)

find_package(Qt5 COMPONENTS
        Core
        Gui
        Widgets
        Multimedia
        REQUIRED)

add_library(sso sso/sso.cpp ServerAuth/ServerAuth.cpp AudioServer/AudioServer.cpp)


add_library(protolib proto/hello.proto proto/sso.proto proto/auth.proto)
target_link_libraries(protolib gRPC::grpc++)

target_include_directories(protolib PUBLIC ${CMAKE_CURRENT_BINARY_DIR})
get_target_property(grpc_cpp_plugin_location gRPC::grpc_cpp_plugin LOCATION)
protobuf_generate(TARGET protolib LANGUAGE cpp)
protobuf_generate(TARGET protolib LANGUAGE grpc
        GENERATE_EXTENSIONS .grpc.pb.h .grpc.pb.cc
        PLUGIN "protoc-gen-grpc=${grpc_cpp_plugin_location}"
)

add_executable(qtui main.cpp
        ./UI/player.cpp
        ./UI/player.h
        ./UI/player.ui
        ./UI/login.cpp
        ./UI/login.h
        ./UI/Login.ui
        ./UI/signup.h
        ./UI/signup.cpp
        ./UI/Worker.h
        ./UI/Streamer.h
        )
target_link_libraries(qtui
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
        Qt5::Multimedia
)

target_link_libraries(sso PUBLIC protolib jwt-cpp::jwt-cpp SQLite::SQLite3 yaml-cpp::yaml-cpp)
target_link_libraries(qtui sso)


