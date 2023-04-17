/*
** backend, 2022
** DynamicLibraryException.cpp by 0xMemoryGrinder
*/

#include "exceptions/DynamicLibraryException.hpp"

osmium::moduleTester::DynamicLibraryException::DynamicLibraryException(std::string reason,
                                                       std::string libPath,
                                                       std::string error)
    : _libPath(std::move(libPath))
{
    _formattedMessage = "[DynamicLibraryException] " + reason + " while interacting with dynamic library at path " + _libPath + ": " + error;
}

const char *osmium::moduleTester::DynamicLibraryException::what() const noexcept
{
    return _formattedMessage.c_str();
}
