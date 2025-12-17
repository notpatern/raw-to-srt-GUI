cmake_minimum_required(VERSION 3.10)

project(raw-to-srt-GUI VERSION 0.0)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

file(GLOB_RECURSE SRC_FILES "${CMAKE_SOURCE_DIR}/${PROJECT_NAME}/src/**.cpp"
    "${CMAKE_SOURCE_DIR}/${PROJECT_NAME}/src/**.h")

include_directories("${CMAKE_SOURCE_DIR}/${PROJECT_NAME}/vendors/imgui"
    "${CMAKE_SOURCE_DIR}/${PROJECT_NAME}/vendors/raw-to-srt"
    "${CMAKE_SOURCE_DIR}/${PROJECT_NAME}/vendors/nlohmann")

add_executable(${PROJECT_NAME} ${SRC_FILES})
