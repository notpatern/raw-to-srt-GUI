cmake_minimum_required(VERSION 3.10)

project(raw-to-srt-GUI VERSION 0.0)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(Vulkan REQUIRED)
find_package(glfw3 CONFIG REQUIRED)

set(IMGUI_DIR "${CMAKE_SOURCE_DIR}/${PROJECT_NAME}/vendors/imgui")
file(GLOB_RECURSE IMGUI_SOURCES
    "${IMGUI_DIR}/*.cpp"
)

set(TINYPROCESS_DIR "${CMAKE_SOURCE_DIR}/${PROJECT_NAME}/vendors/TinyProcessLib")
file(GLOB_RECURSE TINYPROCESS_SOURCES
    "${TINYPROCESS_DIR}/*.cpp"
)

file(GLOB_RECURSE SRC_FILES
    "${CMAKE_SOURCE_DIR}/${PROJECT_NAME}/src/*.cpp"
    "${CMAKE_SOURCE_DIR}/${PROJECT_NAME}/src/*.h"
)

add_executable(${PROJECT_NAME} ${SRC_FILES} ${IMGUI_SOURCES} ${TINYPROCESS_SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE
    "${CMAKE_SOURCE_DIR}/${PROJECT_NAME}/vendors/imgui"
    "${CMAKE_SOURCE_DIR}/${PROJECT_NAME}/vendors/GLFW"
    "${CMAKE_SOURCE_DIR}/${PROJECT_NAME}/vendors/raw-to-srt"
    "${CMAKE_SOURCE_DIR}/${PROJECT_NAME}/vendors/nlohmann"
    "${CMAKE_SOURCE_DIR}/${PROJECT_NAME}/vendors/TinyProcessLib"
)

target_link_libraries(${PROJECT_NAME} PRIVATE Vulkan::Vulkan glfw)
