cmake_minimum_required(VERSION 3.13) 
project(animalsbackend) 

set(CMAKE_CXX_STANDARD 17) 

add_library(restbed STATIC ./restbed/source/corvusoft/restbed)
set_target_properties(restbed PROPERTIES LINKER_LANGUAGE CXX)

add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} restbed)
target_include_directories(${PROJECT_NAME} PUBLIC ./restbed/source/corvusoft/restbed)