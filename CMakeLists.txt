cmake_minimum_required(VERSION 3.10)
project(laaS)
 
set(CMAKE_CXX_STANDARD 11)
 
add_subdirectory(lib)
include_directories("./")
link_directories("./lib")

add_executable(laaS main.cpp)  #生成一个可执行的文件
 
target_link_libraries(laaS library_shared)