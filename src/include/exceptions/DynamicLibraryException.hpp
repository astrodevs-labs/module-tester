/*
** backend, 2022
** DynamicLibraryLoadingException.hpp by 0xMemoryGrinder
*/

#pragma once

#include <iostream>
#include <exception>

namespace osmium::moduleTester {

    /**
     * @class Exception raised when an error occur when interacting with a dynamic library
     */
    class DynamicLibraryException : public std::exception
    {

////////////////////// CONSTRUCTORS/DESTRUCTORS /////////////////////////

        public:
            /**
             * @brief Generic constructor for all Dynamic libraries related exceptions
             * @param reason A brief title of the exception, set by the derived excpetions
             * @param libPath The path of the dynamic library which produces the error
             * @param error A detailed error message provided by the system library (dlopen/dlsym or LoadLibrary/LoadSymbol)
             */
            DynamicLibraryException(std::string reason, std::string libPath, std::string error);

            ~DynamicLibraryException() override = default;


//////////////////////--------------------------/////////////////////////



///////////////////////////// PROPERTIES ////////////////////////////////
        public:


        private:
            /**
             * @property _libPath Path of the library which encounters errors
             */
            std::string _libPath;

            /**
             * @property _formattedMessage Error message aggregating all the constructor arguments
             */
            std::string _formattedMessage;

//////////////////////--------------------------/////////////////////////



/////////////////////////////// METHODS /////////////////////////////////
        public:
            const char *what() const noexcept override;

        private:


//////////////////////--------------------------/////////////////////////

    };

}
