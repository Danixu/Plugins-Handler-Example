#include "plugin.hpp"
#include <iostream>

class SHARED_EXPORT Plugin1 : public Plugin
{
public:
    virtual std::string command(std::string command, std::string options)
    {
        return command + " " + options;
    }
};

DEFINE_PLUGIN(Plugin1, PTInput, "Plugin1", "0.0.1")