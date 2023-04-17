/*
** Project, 2022
** DynamicLoader.cpp by 0xMemoryGrinder
*/

#include <filesystem>
#include "utils/DynamicLoader.hpp"
#include "exceptions/DynamicLibraryException.hpp"
#include "exceptions/MissingDynamicLibraryException.hpp"
#include "exceptions/CorruptedDynamicLibraryException.hpp"

osmium::moduleTester::DynamicLoader::DynamicLoader(DynamicLoader &&ref) noexcept
        : _handler(ref._handler), _libPath(ref._libPath)
{
    ref._handler = nullptr;
}

osmium::moduleTester::DynamicLoader::~DynamicLoader()
{
    if (_handler != nullptr)
        _closeHandle();
    _handler = nullptr;
}

#ifdef _WIN32
std::string osmium::moduleTester::DynamicLoader::_getLastErrorStdStr()
{
    DWORD error = GetLastError();
    if (error)
    {
        LPVOID lpMsgBuf;
        DWORD bufLen = FormatMessage(
                FORMAT_MESSAGE_ALLOCATE_BUFFER |
                FORMAT_MESSAGE_FROM_SYSTEM |
                FORMAT_MESSAGE_IGNORE_INSERTS,
                NULL,
                error,
                MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                (LPTSTR) &lpMsgBuf,
                0, NULL );
        if (bufLen)
        {
            LPCSTR lpMsgStr = (LPCSTR)lpMsgBuf;
            std::string result(lpMsgStr, lpMsgStr+bufLen);

            LocalFree(lpMsgBuf);

            return result;
        }
    }
    return std::string();
}
#endif

void osmium::moduleTester::DynamicLoader::loadHandler(const std::string &libPath)
{
    if (_handler != nullptr)
        _closeHandle();

#if _WIN32
    _handler = LoadLibrary(TEXT(libPath.c_str()));
#else
    _handler = dlopen(libPath.c_str(), RTLD_LAZY | RTLD_GLOBAL);
#endif

    if (_handler == nullptr) {
        if (std::filesystem::exists(libPath))
            throw osmium::moduleTester::MissingDynamicLibraryException(libPath);
#if _WIN32
        throw osmium::moduleTester::CorruptedDynamicLibraryException(libPath, DynamicLoader::_getLastErrorStdStr());
#else
        throw osmium::moduleTester::CorruptedDynamicLibraryException(libPath, dlerror());
#endif
    }
    _libPath = libPath;
}

void osmium::moduleTester::DynamicLoader::_closeHandle()
{
#if _WIN32
    FreeLibrary((HMODULE)_handler);
#else
    dlclose(_handler);
#endif
}
