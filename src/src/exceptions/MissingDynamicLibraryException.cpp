/*
** backend, 2022
** MissingDynamicLibraryException.cpp by 0xMemoryGrinder
*/

#include "exceptions/MissingDynamicLibraryException.hpp"

osmium::moduleTester::MissingDynamicLibraryException::MissingDynamicLibraryException(std::string libPath)
    : DynamicLibraryException("Missing dynamic library", std::move(libPath), "No such file or directory")
{
}