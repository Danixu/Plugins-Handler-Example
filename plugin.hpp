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
        std::shared_ptr<Plugin> load()                      \
        {                                                   \
            return std::make_shared<classType>();           \
        }                                                   \
                                                            \
        const char *name()                                  \
        {                                                   \
            return pluginName;                              \
        }                                                   \
                                                            \
        const char *version()                               \
        {                                                   \
            return pluginVersion;                           \
        }                                                   \
    }
