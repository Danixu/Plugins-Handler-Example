#include <cstring>
#include <string>
#include <vector>
#include <filesystem>
#include <memory>

#include "plugin.hpp"

#ifndef _PLUGIN_HANDLER_H_
#define _PLUGIN_HANDLER_H_

class PluginHandler
{
    using allocClass = Plugin *(*)();
    using charPReturn = char *(*)();
    using PTReturn = PluginType (*)();

    allocClass _load = NULL;
    PTReturn _get_type;
    charPReturn _get_name = NULL;
    charPReturn _get_version = NULL;
    char *last_error = NULL;

    void *handle = NULL;
    Plugin *instance = NULL;

    void set_error(char *error_text);

public:
    PluginHandler(std::string name);
    ~PluginHandler();

    std::string get_name();
    std::string get_version();
    Plugin *load();
    PluginType get_type();
    bool has_error();
    char *get_error();
    void clear_error();
};

std::vector<std::shared_ptr<PluginHandler>> load_plugins(std::string path, std::string extension, PluginType type = (PluginType)0);
#endif // _PLUGIN_HANDLER_H_