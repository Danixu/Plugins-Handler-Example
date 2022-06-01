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
    PTAll = 0,
    PTInput,
    PTOutput
};

class Plugin
{
public:
    Plugin() = default;
    virtual ~Plugin() = default;
    virtual std::string command(std::string command, std::string options) = 0;
};

#define DEFINE_PLUGIN(classType, PType, pluginName, pluginVersion) \
    extern "C"                                                     \
    {                                                              \
        classType SHARED_EXPORT *load()                            \
        {                                                          \
            return new classType();                                \
        }                                                          \
                                                                   \
        void SHARED_EXPORT unload(classType *ptr)                  \
        {                                                          \
            delete ptr;                                            \
        }                                                          \
                                                                   \
        PluginType SHARED_EXPORT get_type()                        \
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