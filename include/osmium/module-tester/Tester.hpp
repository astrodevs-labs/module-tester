/*
** module_tester, 2022
** Tester.hpp by 0xMemoryGrinder
*/

#pragma once

#include <memory>
#include <vector>
#include <functional>
#include <typeindex>
#include <any>
#include <future>
#include "types.hpp"

#define TESTER_EXTRACT_TYPE(...) std::type_index(typeid(__VA_ARGS__))

namespace osmium::moduleTester
{
    class Tester
    {

////////////////////// CONSTRUCTORS/DESTRUCTORS /////////////////////////

        public:
            virtual ~Tester() = default;


//////////////////////--------------------------/////////////////////////



///////////////////////////// PROPERTIES ////////////////////////////////
        public:
            using message_type = std::type_index;


        private:


//////////////////////--------------------------/////////////////////////



/////////////////////////////// METHODS /////////////////////////////////
        public:
            static std::shared_ptr<Tester> create();

            template<class T>
            bool toHaveEmitted(std::shared_ptr<T> message)
            {
                return _toHaveEmitted(TESTER_EXTRACT_TYPE(T), [message](std::any anyMessage) {
                    auto castedMessage = std::any_cast<std::shared_ptr<T>>(anyMessage);

                    if ((castedMessage == nullptr && message != nullptr) || (castedMessage != nullptr && message == nullptr))
                        return false;
                    if (castedMessage == nullptr && message == nullptr)
                        return true;
                    return *message == *castedMessage;
                });
            }

            template<class T>
            bool toHaveEmitted(std::vector<std::shared_ptr<T>> messages)
            {
                for (auto msg: messages) {
                    if (!toHaveEmitted(msg))
                        return false;
                }
                return true;
            }

            template<class T>
            handler_id mockHandler(uint64_t priority, std::function<void(std::shared_ptr<T>, std::string)> handler)
            {
                return _registerHandler(TESTER_EXTRACT_TYPE(T), priority, [handler](std::any message, std::string moduleName) {
                    auto payload = std::any_cast<std::shared_ptr<T>>(message);
                    handler(payload, moduleName);
                });
            }

            template<class T>
            handler_id mockHandler(std::shared_ptr<T> response)
            {
                return _registerHandler(TESTER_EXTRACT_TYPE(T), 0, [response](std::any message, std::string moduleName) {
                    auto payload = std::any_cast<std::shared_ptr<T>>(message);
                    *payload = *response;
                });
            }

            /**
             * @brief Emit a message then forward it to all the modules that registered handlers for this message type
             * @tparam T The type of the message to emit
             * @param message The message to emit
             * @param moduleName The name of the module which emitted the message
             * @return A future that will be resolved when all the handlers have been called. The future will contain the message that was emitted and mutations if any were made by the handlers.
             */
            template<typename T>
            std::future<std::shared_ptr<T>> emit(std::shared_ptr<T> message, std::string moduleName)
            {
                return std::any_cast<std::future<std::shared_ptr<T>>>(
                    _emitMessage(TESTER_EXTRACT_TYPE(T), [moduleName, message](std::function<void(std::any, std::string)> handlerCaller) {
                        handlerCaller(message, moduleName);
                    })
                );
            }

            virtual void loadModule(std::string path) = 0;

            virtual bool toHaveHandlerRegistered(uint64_t priority, message_type type) = 0;

            virtual bool toHaveHandlersRegistered(int nbRegistered, message_type type) = 0;

            virtual void removeHandler(handler_id id) = 0;

        private:
            virtual bool _toHaveEmitted(message_type type, std::function<bool(std::any)> predicate) = 0;

            virtual handler_id _registerHandler(message_type type, uint64_t priority, std::function<void(std::any, std::string)> handler) = 0;

            virtual void _emitMessage(message_type type, std::function<void(std::function<void(std::any, std::string)>)> handlerCaller) = 0;

//////////////////////--------------------------/////////////////////////

    };

} // tester