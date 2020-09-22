cmake_minimum_required(VERSION 3.10)
project(library)
 
set(CMAKE_CXX_STANDARD 11)
 
add_library(library_shared Clock.cpp RWLock.cpp)