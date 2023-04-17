/*
** module-tester, 2023
** GoodTester.cpp by 0xMemoryGrinder
*/

#include <gtest/gtest.h>
#include "osmium/module-tester/Tester.hpp"

TEST(CorrectBehavior, CorrectBehavior)
{
    std::shared_ptr<osmium::moduleTester::Tester> tester = osmium::moduleTester::Tester::create();
#ifdef _WIN32
    tester->loadModule("inputModule/module-tester-tests-input-module.dll");
#else
    tester->loadModule("inputModule/module-tester-tests-input-module.so");
#endif

    ASSERT_TRUE(tester->toHaveHandlerRegistered(1000, TESTER_EXTRACT_TYPE(std::string)));
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    ASSERT_TRUE(tester->toHaveEmitted(std::make_shared<std::string>("Hello World!")));
}