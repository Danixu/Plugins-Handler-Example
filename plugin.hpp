/*

  This header file is the virtual plugin definition which will be used in derivated plugins and main program

*/

#include "export.h"
#include <string>
#include <memory>

class Plugin
{
public:
    Plugin(){};
    virtual ~Plugin(){};
    virtual std::string command(std::string command, std::string options) { return ""; }
};

#define DEFINE_PLUGIN(classType, pluginName, pluginVersion) \
    extern "C"                                              \
    {                                                       \
        std::shared_ptr<Plugin> SHARED_EXPORT load()        \
        {                                                   \
            return std::make_shared<classType>();           \
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
