/*
** module_tester, 2023
** MessagingInterface.hpp by 0xMemoryGrinder
*/

#pragma once

#include <memory>
#include <functional>
#include <future>
#include "TesterImpl.hpp"

namespace osmium::moduleTester
{

    class MessagingInterface
    {

////////////////////// CONSTRUCTORS/DESTRUCTORS /////////////////////////

        public:
            explicit MessagingInterface(std::shared_ptr<TesterImpl> tester);

            ~MessagingInterface() = default;


//////////////////////--------------------------/////////////////////////



///////////////////////////// PROPERTIES ////////////////////////////////
        public:


        private:
            std::shared_ptr<TesterImpl> _tester;


//////////////////////--------------------------/////////////////////////



/////////////////////////////// METHODS /////////////////////////////////
        public:
            /**
             * @brief Register a handler for a specific message type, the handler will be called when a message of the same type is emitted
             * @tparam T The type of the message to handle
             * @param priority A priority to sort the handlers, the higher the priority, the later the handler will be called (0 is the highest priority). If two handlers have the same priority, the order is undefined. You should leave space between priorities to allow other modules to register handlers between yours.
             * @param handler The handler to register. The handler will be called with the message and the name of the module that emitted it.
             * @return The ID of the registered handler
             */
            template<typename T>
            handler_id registerHandler(uint64_t priority, std::function<void(std::shared_ptr<T>, std::string)> handler)
            {
                return _tester->registerHandler<T>(priority, handler);
            }

            /**
             * @brief Remove the handler identified by its id from _handlers.
             * @param id The id of the handler to remove.
             */
            void removeHandler(handler_id id)
            {
                _tester->removeHandler(id);
            }

            /**
             * @brief Emit a message to the core, the core will then forward it to all the modules that registered handlers for this message type
             * @tparam T The type of the message to emit
             * @param message The message to emit
             * @return A future that will be resolved when all the handlers have been called. The future will contain the message that was emitted and mutations if any were made by the handlers.
             */
            template<typename T>
            std::future<std::shared_ptr<T>> emitMessage(std::shared_ptr<T> message)
            {
                return _tester->emitMessage<T>(message);
            }


        private:


//////////////////////--------------------------/////////////////////////

    };
}
