#pragma once

/*

  This header file is the virtual plugin definition which will be used in derivated plugins and main program

*/

#include "export.h"
#include <string>
#include <memory>

class Plugin
{
public:
    virtual ~Plugin() = default;
    virtual std::string command(std::string command, std::string options) = 0;
};

#define DEFINE_PLUGIN(classType, pluginName, pluginVersion) \
    extern "C"                                              \
    {                                                       \
        SHARED_EXPORT classType *load()                     \
        {                                                   \
            printf("Creating new class pointer\n");         \
            return new classType();                         \
        }                                                   \
                                                            \
        SHARED_EXPORT void unload(classType *ptr)           \
        {                                                   \
            delete ptr;                                     \
        }                                                   \
                                                            \
        const char SHARED_EXPORT *name()                    \
        {                                                   \
            return pluginName;                              \
        }                                                   \
                                                            \
        const char SHARED_EXPORT *version()                 \
        {                                                   \
            return pluginVersion;                           \
        }                                                   \
    }
