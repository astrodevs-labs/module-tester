/*
** backend, 2022
** CorruptedDynamicLibraryException.cpp by 0xMemoryGrinder
*/

#include "exceptions/CorruptedDynamicLibraryException.hpp"

osmium::moduleTester::CorruptedDynamicLibraryException::CorruptedDynamicLibraryException(std::string libPath, std::string message)
    : DynamicLibraryException("Corrupted dynamic library", std::move(libPath), std::move(message))
{
}