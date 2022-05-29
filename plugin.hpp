#pragma once

/*

  This header file is the virtual plugin definition which will be used in derivated plugins and main program

*/

#include "export.h"
#include <string>
#include <memory>

#ifndef _PLUGIN_HPP_H_
#define _PLUGIN_HPP_H_
enum PluginType
{
    PTInput = 1,
    PTOutput
};

class Plugin
{
public:
    virtual ~Plugin() = default;
    virtual std::string command(std::string command, std::string options) = 0;
};

#define DEFINE_PLUGIN(classType, PType, pluginName, pluginVersion) \
    extern "C"                                                     \
    {                                                              \
        SHARED_EXPORT classType *load()                            \
        {                                                          \
            printf("Creating new class pointer\n");                \
            return new classType();                                \
        }                                                          \
                                                                   \
        SHARED_EXPORT void unload(classType *ptr)                  \
        {                                                          \
            delete ptr;                                            \
        }                                                          \
                                                                   \
        SHARED_EXPORT PluginType get_type()                        \
        {                                                          \
            return PType;                                          \
        }                                                          \
                                                                   \
        const char SHARED_EXPORT *name()                           \
        {                                                          \
            return pluginName;                                     \
        }                                                          \
                                                                   \
        const char SHARED_EXPORT *version()                        \
        {                                                          \
            return pluginVersion;                                  \
        }                                                          \
    }

#endif // _PLUGIN_HPP_H_