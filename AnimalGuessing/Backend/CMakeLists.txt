cmake_minimum_required(VERSION 3.13) 
project(animalsbackend) 

set(CMAKE_CXX_STANDARD 17) 

add_executable(${PROJECT_NAME} Main.cpp NetworkHandler.cpp)

target_include_directories(${PROJECT_NAME} 
    PUBLIC restbed/source/corvusoft/restbed
    PUBLIC restbed/source
)

target_link_libraries(${PROJECT_NAME} ../restbed/build/Release/restbed)
target_link_libraries(${PROJECT_NAME} ../restbed/build/Release/restbed-shared)