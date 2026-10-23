cmake_minimum_required(VERSION 3.16)
project(armor_detection LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(OpenCV_DIR "/usr/local/opencv4.5.4/lib/cmake/opencv4")
find_package(OpenCV 4.5.4 EXACT REQUIRED)

add_executable(armor_detection src/main.cpp)

target_include_directories(armor_detection PRIVATE ${OpenCV_INCLUDE_DIRS})
target_link_libraries(armor_detection PRIVATE ${OpenCV_LIBS})