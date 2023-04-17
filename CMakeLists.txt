cmake_minimum_required(VERSION 3.24)
project(module-tester)

set(CMAKE_CXX_STANDARD 20)

file(GLOB_RECURSE SRCS src/*.cpp)

file(GLOB_RECURSE ICLS include/*.hpp)
file(GLOB_RECURSE PRIV_ICLS src/include/*.hpp)

add_library(module-tester ${SRCS} ${ICLS} ${PRIV_ICLS})
target_include_directories(module-tester PUBLIC include)
target_include_directories(module-tester PRIVATE src/include)
set_target_properties(module-tester PROPERTIES POSITION_INDEPENDENT_CODE ON)

if (${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_CURRENT_SOURCE_DIR})
    include(tests/cmake/FindGoogleTest.cmake)
    enable_testing()

    add_subdirectory(tests)
endif ()