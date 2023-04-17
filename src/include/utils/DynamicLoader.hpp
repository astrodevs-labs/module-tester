/*
** Project, 2022
** DynamicLoader.hpp by 0xMemoryGrinder
*/

#pragma once
#include <iostream>

#ifdef _WIN32
    #include <Windows.h>
#else
    #include <dlfcn.h> //dlopen
#endif
#include "exceptions/DynamicLibraryException.hpp"

namespace osmium::moduleTester
{

    class DynamicLoader
    {

////////////////////// CONSTRUCTORS/DESTRUCTORS /////////////////////////

        public:
            DynamicLoader() = default;

            DynamicLoader(const DynamicLoader&) = delete;
            DynamicLoader(DynamicLoader&&) noexcept;

            ~DynamicLoader();


//////////////////////--------------------------/////////////////////////



///////////////////////////// PROPERTIES ////////////////////////////////
        public:


        protected:
#ifdef _WIN32
            /**
             * @property _handler pointer to opened dynamic library
             */
            HINSTANCE _handler = nullptr;
#else
            /**
             * @property _handler pointer to opened dynamic library
             */
            void *_handler = nullptr;
#endif

            /**
             * @property _libPath path to the dynamic library
             */
            std::string _libPath;


//////////////////////--------------------------/////////////////////////



/////////////////////////////// METHODS /////////////////////////////////
        public:
            /**
             * @details Close the previous opened library (if one is open) and opens the one passed as parameter
             * @param libPath
             * @throws MissingDynamicLibraryException if the library does not exists at the provided path
             * @throws CorruptedDynamicLibraryException if the library cannot be opened
             */
            void loadHandler(const std::string &libPath);

            /**
             * @brief Search for a symbol in the opened library and if found, return it with the specified type signature
             * @tparam T type of the symbol to return (generally a function pointer)
             * @param name name of the symbol to search for
             * @param no_except if true, the function will return a nullptr if the symbol is not found and not throw an exception
             * @throws DynamicLibraryException if the symbol is not found and no_except is false
             * @return the symbol with the specified type signature
             */
            template<typename T>
            T loadSymbol(std::string name, bool no_except = false)
            {
#ifdef _WIN32
                T s = (T) GetProcAddress(_handler, name.c_str());
#else
                void *s = dlsym(_handler, name.c_str());
#endif
                if (s == nullptr && !no_except)
#ifdef _WIN32
                    throw osmium::moduleTester::DynamicLibraryException("Cannot find symbol " + name, _libPath, DynamicLoader::_getLastErrorStdStr());
#else
                    throw osmium::moduleTester::DynamicLibraryException("Cannot find symbol \"" + name + "\"", _libPath, dlerror());
#endif
                return reinterpret_cast<T>(s);
            }

        protected:
            void _closeHandle();

        private:
#ifdef _WIN32
            static std::string _getLastErrorStdStr();
#endif


//////////////////////--------------------------/////////////////////////

    };
}

