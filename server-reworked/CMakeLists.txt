cmake_minimum_required(VERSION 3.20)
project(server)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(yaml-cpp CONFIG REQUIRED)
find_package(libpqxx REQUIRED)
find_package(PostgreSQL REQUIRED)

add_library(libs STATIC
 src/Config/Config.cpp
 src/Database/Database.cpp
 )

target_include_directories(libs PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_include_directories(libs PUBLIC ${CMAKE_CURRENT_BINARY_DIR})
target_include_directories(libs PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/build)

include(FetchContent)

FetchContent_Declare(
    spdlog
    GIT_REPOSITORY https://github.com/gabime/spdlog.git
    GIT_TAG        v1.x
)

FetchContent_MakeAvailable(spdlog)

add_executable(server src/main.cpp)

target_include_directories(server PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_include_directories(server PUBLIC ${CMAKE_CURRENT_BINARY_DIR})
target_include_directories(server PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/build)

target_link_libraries(libs PUBLIC spdlog yaml-cpp::yaml-cpp pqxx pq PostgreSQL::PostgreSQL) 
target_link_libraries(server PRIVATE libs)