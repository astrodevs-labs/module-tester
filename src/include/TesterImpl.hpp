/*
** module_tester, 2023
** TesterImpl.hpp by 0xMemoryGrinder
*/

#pragma once

#include <random>
#include <future>
#include "osmium/module-tester/Tester.hpp"
#include "osmium/module-tester/types.hpp"
#include "utils/DynamicLoader.hpp"

namespace osmium::moduleTester
{

    class TesterImpl : public Tester, public std::enable_shared_from_this<TesterImpl>
    {

////////////////////// CONSTRUCTORS/DESTRUCTORS /////////////////////////

        public:
            TesterImpl() = default;

            ~TesterImpl() override = default;


//////////////////////--------------------------/////////////////////////



///////////////////////////// PROPERTIES ////////////////////////////////
        public:


        private:
            std::unordered_map<message_type, std::unordered_map<uint64_t, std::unordered_map<handler_id, std::any>>> _handlers;
            std::unordered_map<message_type, std::vector<std::any>> _emitted;
            std::unique_ptr<std::any> _module;
            std::string _moduleName;
            DynamicLoader _loader;


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
                handler_id id = _generateHandlerId();

                _handlers[std::type_index(typeid(T))][priority][id] = [handler](std::any anyMessage, std::string moduleName) {
                    handler(std::any_cast<std::shared_ptr<T>>(anyMessage), moduleName);
                };
                return id;
            }

            /**
             * @brief Remove the handler identified by its id from _handlers.
             * @param id The id of the handler to remove.
             */
            void removeHandler(handler_id id) override
            {
                for (auto &typeIndex: _handlers) {
                    for (auto &priority : _handlers[typeIndex.first]) {
                        for (auto &handlerId : priority.second) {
                            if (handlerId.first == id) {
                                priority.second.erase(handlerId.first);
                                return;
                            }
                        }
                    }
                }
            }

            /**
             * @brief Emit a message to the module
             * @tparam T The type of the message to emit
             * @param message The message to emit
             * @return A future that will be resolved when all the handlers have been called. The future will contain the message that was emitted and mutations if any were made by the handlers.
             */
            template<typename T>
            std::future<std::shared_ptr<T>> emitMessage(std::shared_ptr<T> message)
            {
                _emitted[std::type_index(typeid(T))].push_back(message);

                return std::async(std::launch::async, [this, message]() {
                    for (auto &priority : _handlers[std::type_index(typeid(T))]) {
                        for (auto &handlerId : priority.second) {
                            std::any_cast<std::function<void(std::shared_ptr<T>, std::string)>>(handlerId.second)(message, _moduleName);
                        }
                    }
                    return message;
                });
            }

            void loadModule(std::string path) override;

            bool toHaveHandlerRegistered(uint64_t priority, message_type type) override;

            bool toHaveHandlersRegistered(int nbRegistered, message_type type) override;

        private:
            handler_id _registerHandler(message_type type, uint64_t priority,
                    std::function<void(std::any, std::string)> handler) override;

            void _emitMessage(message_type type, std::function<void(
                    std::function<void(std::any, std::string)>)> handlerCaller) override;

            static std::string _generateHandlerId()
            {
                static std::random_device dev;
                static std::mt19937_64 rng(dev());
                static std::uniform_int_distribution<std::uint64_t> dist(0, 18446744073709551615u);

                std::uint64_t data[2];
                data[0] = dist(rng);
                data[1] = dist(rng);

                // Set the version number to 4 (randomly generated)
                data[0] &= ~(0xf000ull);
                data[0] |= 0x4000ull;

                // Set the variant to RFC 4122
                data[1] &= ~(0xc000'0000'0000'0000ull);
                data[1] |= 0x8000'0000'0000'0000ull;

                // Format the UUID string
                char buf[37];
                std::snprintf(buf, sizeof(buf),
                              "%08x-%04x-%04x-%04x-%012llx",
                              (unsigned int)(data[0] >> 32),
                              (unsigned int)(data[0] >> 16),
                              (unsigned int)(data[0] >> 0),
                              (unsigned int)(data[1] >> 48),
                              (unsigned long long)(data[1] & 0x0000'ffff'ffff'ffffull));

                return {buf};
            }

            bool _toHaveEmitted(message_type type, std::function<bool(std::any)> predicate) override;




//////////////////////--------------------------/////////////////////////

    };
}
