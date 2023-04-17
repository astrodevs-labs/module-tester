cmake_minimum_required(VERSION 3.17)

set(CMAKE_CXX_STANDARD 20)

file(GLOB_RECURSE SRCS src/*.cpp)
file(GLOB_RECURSE ICLS include/*.hpp)


#create a new target
add_library(module-tester-tests-input-module SHARED  ${SRCS} ${ICLS})
set_target_properties(module-tester-tests-input-module PROPERTIES PREFIX "")
#set the include directories
target_include_directories(module-tester-tests-input-module PUBLIC include ../../include ../../src/include)