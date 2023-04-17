/*
** backend, 2023
** InputModule.hpp by 0xMemoryGrinder
*/

#pragma once

#include <memory>
#include "../../../src/include/MessagingInterface.hpp"

class InputModule
{

////////////////////// CONSTRUCTORS/DESTRUCTORS /////////////////////////

    public:
        InputModule(
                std::shared_ptr<osmium::moduleTester::MessagingInterface> msg)
                : _msg(msg)
        {
            _msg->registerHandler<std::string>(1000,
                                                    [](std::shared_ptr<std::string> received,
                                                       std::string from) {
                                                        std::cout
                                                                << "Received: "
                                                                << *received
                                                                << " from: "
                                                                << from
                                                                << std::endl;
                                                    });

            std::async(std::launch::async, [this]() {
                std::this_thread::sleep_for(std::chrono::seconds(1));
                _msg->emitMessage(std::make_shared<std::string>("Hello World!"));
            });
        }

        ~InputModule() = default;


//////////////////////--------------------------/////////////////////////



///////////////////////////// PROPERTIES ////////////////////////////////
    public:
        std::shared_ptr<osmium::moduleTester::MessagingInterface> _msg;


    private:


//////////////////////--------------------------/////////////////////////



/////////////////////////////// METHODS /////////////////////////////////
    public:


    private:


//////////////////////--------------------------/////////////////////////

};
