/*
** module_tester, 2022
** TesterImpl.cpp by 0xMemoryGrinder
*/


#include <random>
#include <algorithm>
#include "TesterImpl.hpp"
#include "MessagingInterface.hpp"

std::shared_ptr<osmium::moduleTester::Tester> osmium::moduleTester::Tester::create()
{
    return std::make_shared<TesterImpl>();
}

bool osmium::moduleTester::TesterImpl::_toHaveEmitted(
        osmium::moduleTester::Tester::message_type type,
        std::function<bool(std::any)> predicate)
{
    if (_emitted.find(type) == _emitted.end())
        return false;
    return std::any_of(_emitted[type].begin(), _emitted[type].end(), predicate);
}

void osmium::moduleTester::TesterImpl::loadModule(std::string path)
{
    _loader.loadHandler(path);

    auto createModuleFct = _loader.loadSymbol<void *(*)(std::shared_ptr<MessagingInterface>)>("createModule");
    auto getModuleNameFct = _loader.loadSymbol<const char *(*)()>("getModuleName");
    _moduleName = std::string(getModuleNameFct());
    auto shared = shared_from_this();
    auto msgInterface = std::make_shared<MessagingInterface>(shared);
    auto module = reinterpret_cast<std::any *>(createModuleFct(msgInterface));

    _module = std::unique_ptr<std::any>(module);
}

bool
osmium::moduleTester::TesterImpl::toHaveHandlerRegistered(uint64_t priority,
                                                          osmium::moduleTester::Tester::message_type type)
{
    if (_handlers.find(type) == _handlers.end())
        return false;
    return _handlers[type].find(priority) != _handlers[type].end();
}

bool
osmium::moduleTester::TesterImpl::toHaveHandlersRegistered(int nbRegistered,
                                                           osmium::moduleTester::Tester::message_type type)
{
    if (_handlers.find(type) == _handlers.end())
        return false;
    return _handlers[type].size() == nbRegistered;
}

osmium::moduleTester::handler_id
osmium::moduleTester::TesterImpl::_registerHandler(
        osmium::moduleTester::Tester::message_type type, uint64_t priority,
        std::function<void(std::any, std::string)> handler)
{
    auto id = _generateHandlerId();

    _handlers[type][priority][id] = handler;
    return id;
}

void osmium::moduleTester::TesterImpl::_emitMessage(
        osmium::moduleTester::Tester::message_type type, std::function<void(
        std::function<void(std::any, std::string)>)> handlerCaller)
{
    for (auto [priority, handlers] : _handlers[type]) {
        for (auto [id, handler] : handlers) {
            handlerCaller(std::any_cast<std::function<void(std::any, std::string)>>(handler));
        }
    }
}
