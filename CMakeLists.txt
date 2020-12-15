cmake_minimum_required(VERSION 3.15)
project(laaS)
 
set(CMAKE_CXX_STANDARD 11)

add_compile_options(-std=c++17 -finput-charset=UTF-8 -fexec-charset=GBK -I C:\\boost_1_74_0)
add_subdirectory(lib)
include_directories("./")
link_directories("./lib")

add_executable(laaS main.cpp)  #生成一个可执行的文件
 
target_link_libraries(laaS library_shared)