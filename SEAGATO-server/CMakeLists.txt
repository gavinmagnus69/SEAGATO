cmake_minimum_required(VERSION 3.20)
project(server-sg)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

#jwt-decoder
find_package(jwt-cpp CONFIG REQUIRED)

#sqlite
# find_package(SQLite3 CONFIG REQUIRED)

#yaml-cpp
find_package(yaml-cpp CONFIG REQUIRED)

#grpc and protobuf
find_package(Protobuf CONFIG REQUIRED)
find_package(gRPC CONFIG REQUIRED)
add_library(protolib proto/audio.proto proto/auth.proto)
target_link_libraries(protolib gRPC::grpc++)
target_include_directories(protolib PUBLIC ${CMAKE_CURRENT_BINARY_DIR})
get_target_property(grpc_cpp_plugin_location gRPC::grpc_cpp_plugin LOCATION)
protobuf_generate(TARGET protolib LANGUAGE cpp)
protobuf_generate(TARGET protolib LANGUAGE grpc
GENERATE_EXTENSIONS .grpc.pb.h .grpc.pb.cc 
PLUGIN "protoc-gen-grpc=${grpc_cpp_plugin_location}"
)



#сюда всю хуйню классовую
add_library(ent STATIC Entities/Track.cpp Entities/User.cpp Services/Interactor.cpp
Implementations/TgController.cpp Implementations/YaController.cpp
GRPC_Implementations/grpc.cpp GRPC_Implementations/GRPCServer.cpp
Implementations/ServerController.cpp Implementations/DBController.cpp
GRPC_Implementations/GRPCAuth.cpp tools/tools.cpp Config/Config.cpp
Builder/ServiceBuilder.cpp 
)
target_include_directories(ent PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Entities ${CMAKE_CURRENT_SOURCE_DIR}/Services
${CMAKE_CURRENT_SOURCE_DIR}/Implementations ${CMAKE_CURRENT_SOURCE_DIR}/Interfaces
${CMAKE_CURRENT_SOURCE_DIR}/GRPC_Implementations ${CMAKE_CURRENT_SOURCE_DIR}/tools
${CMAKE_CURRENT_SOURCE_DIR}/Config ${CMAKE_CURRENT_SOURCE_DIR}/Builder
)


add_executable(server-sg src/main.cpp)


#и сюда
target_include_directories(server-sg PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Entities ${CMAKE_CURRENT_SOURCE_DIR}/Services
${CMAKE_CURRENT_SOURCE_DIR}/Implementations ${CMAKE_CURRENT_SOURCE_DIR}/Interfaces
${CMAKE_CURRENT_SOURCE_DIR}/GRPC_Implementations ${CMAKE_CURRENT_SOURCE_DIR}/tools
)
target_link_libraries(server-sg PUBLIC ent protolib)
target_include_directories(ent PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/build/proto)
#????
target_link_libraries(ent PUBLIC protolib jwt-cpp::jwt-cpp  yaml-cpp::yaml-cpp)
#target_link_libraries(server-sg protolib)
#SQLite::SQLite3