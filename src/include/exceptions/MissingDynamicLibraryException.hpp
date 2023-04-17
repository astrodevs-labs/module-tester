/*
** backend, 2022
** MissingDynamicLibraryException.hpp by 0xMemoryGrinder
*/

#pragma once
#include <exception>
#include <string>
#include <utility>
#include "DynamicLibraryException.hpp"

namespace osmium::moduleTester
{
    /**
     * @class Exception raised when a dynamic library is not found at specified path
     */
    class MissingDynamicLibraryException : public DynamicLibraryException
    {
        public:
            explicit MissingDynamicLibraryException(std::string libPath);

            ~MissingDynamicLibraryException() override = default;
    };
}