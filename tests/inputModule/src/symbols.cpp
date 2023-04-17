/*
** backend, 2023
** symbols.cpp by 0xMemoryGrinder
*/

#include <string>
#include <memory>
#include "InputModule.hpp"
#include "../../../src/include/MessagingInterface.hpp"

extern "C" {
    const char *getModuleName() {
        return "inputModule";
    }

    void *createModule(std::shared_ptr<osmium::moduleTester::MessagingInterface> msg) {
        return new InputModule(msg);
    }
}