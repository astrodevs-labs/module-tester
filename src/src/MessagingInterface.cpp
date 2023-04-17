/*
** module_tester, 2022
** MessagingInterface.cpp by 0xMemoryGrinder
*/


#include <utility>
#include "MessagingInterface.hpp"

osmium::moduleTester::MessagingInterface::MessagingInterface(
        std::shared_ptr<TesterImpl> tester)
    : _tester(std::move(tester))
{

}
