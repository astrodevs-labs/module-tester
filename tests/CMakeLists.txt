cmake_minimum_required(VERSION 3.17)
set(CMAKE_CXX_STANDARD 20)

file(GLOB_RECURSE E2E_TESTS_SRCS e2e-tests/*.cpp)

add_subdirectory(inputModule)

add_executable(e2e-tests ${E2E_TESTS_SRCS})
target_link_libraries(e2e-tests PRIVATE module-tester GTest::gtest_main)
target_include_directories(e2e-tests PRIVATE ${CMAKE_SOURCE_DIR}/src/include ../src/include)
add_dependencies(e2e-tests module-tester-tests-input-module)

include(GoogleTest)
gtest_discover_tests(e2e-tests)