#ifndef _SHARED_EXPORTS_H__
#define _SHARED_EXPORTS_H__

#ifdef _WIN32
#include <windows.h>
#include <string>

#define LIBLOAD(x) LoadLibrary(x)
#define dlclose(x) FreeLibrary((HMODULE)x)
#define dlsym(x, y) GetProcAddress((HINSTANCE)x, y)
char *dlerror()
{
    DWORD errorMessageID = GetLastError();
    if (errorMessageID == 0)
    {
        return NULL; // No error message has been recorded
    }

// Visual studio is different (as usual)
#if defined _MSC_VER
    LPWSTR messageBuffer = nullptr;
    // Ask Win32 to give us the string version of that message ID.
    // The parameters we pass in, tell Win32 to create the buffer that holds the message for us (because we don't yet know how long the message string will be).
    size_t size = FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                                NULL, errorMessageID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), messageBuffer, 0, NULL);

    // Copy the error message into a std::string.
    std::wstring messageW(messageBuffer, size);
    std::string message(messageW.begin(), messageW.end());
#else
    LPSTR messageBuffer = nullptr;
    // Ask Win32 to give us the string version of that message ID.
    // The parameters we pass in, tell Win32 to create the buffer that holds the message for us (because we don't yet know how long the message string will be).
    size_t size = FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                                NULL, errorMessageID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, NULL);

    // Copy the error message into a std::string.
    std::string message(messageBuffer, size);
#endif

    // Free the Win32's string's buffer.
    LocalFree(messageBuffer);

    char *cstr = new char[message.length() + 1];
    strcpy(cstr, message.c_str());

    // char *cstr = (char *)message.c_str();
    fprintf(stderr, "Error: %s\n\n", cstr);

    return cstr;
}

#ifdef BUILD_LIB
#define SHARED_EXPORT __declspec(dllexport)
#else
#define SHARED_EXPORT __declspec(dllimport)
#endif
#else
#include <dlfcn.h>
#define SHARED_EXPORT
#define LIBLOAD(x) dlopen(x, RTLD_LAZY)
#endif

#endif /* _SHARED_EXPORTS_H__ */