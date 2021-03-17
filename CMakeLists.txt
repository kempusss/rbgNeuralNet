cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
project(test-app)

find_package(Torch REQUIRED)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${TORCH_CXX_FLAGS}")


file(GLOB SOURCES "*.cpp" "torch_modules/src/*.cpp" "state_parsers/src/*.cpp")
add_executable(test-app ${SOURCES})

target_link_libraries(test-app "${TORCH_LIBRARIES}")
set_property(TARGET test-app PROPERTY CXX_STANDARD 17)
target_include_directories(test-app PUBLIC torch_modules/inc/ state_parsers/inc/ ".")