cmake_minimum_required(VERSION 3.10)
#set(CMAKE_CXX_STANDARD 14)
#set(CMAKE_CXX_STANDARD_REQUIRED ON)
# set the project name
project(RaspiFT1060Mpp VERSION 0.1)
set(CMAKE_BUILD_TYPE Release)

include_directories(${PROJECT_SOURCE_DIR}/src ${PROJECT_BINARY_DIR}/include)

add_subdirectory(src)
add_subdirectory(srcTests)
#add_subdirectory(pybind11)
