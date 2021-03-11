cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
project(test-app)

find_package(Torch REQUIRED)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${TORCH_CXX_FLAGS}")

add_executable(test-app test.cpp shallownet.cpp flatlearner.cpp)
target_link_libraries(test-app "${TORCH_LIBRARIES}")
set_property(TARGET test-app PROPERTY CXX_STANDARD 17)