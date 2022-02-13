cmake_minimum_required(VERSION 3.20)
project(lab2_cpp)

set(CMAKE_CXX_STANDARD 14)

set(CURL_LIBRARY "-lcurl")
find_package(CURL REQUIRED)


add_executable(lab2_cpp main.cpp)

include_directories(${CURL_INCLUDE_DIR})
target_link_libraries(lab2_cpp ${CURL_LIBRARIES})
