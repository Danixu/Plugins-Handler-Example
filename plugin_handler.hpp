#include <string>
#include <vector>
#include "plugin.hpp"
#include <filesystem>
#include <memory>

#ifndef _PLUGIN_HANDLER_H_
#define _PLUGIN_HANDLER_H_

class PluginHandler
{
    using allocClass = Plugin *(*)();
    using deleteClass = void (*)(Plugin *);
    using charPReturn = char *(*)();
    using PTReturn = PluginType (*)();

    allocClass _load = NULL;
    deleteClass _unload = NULL;
    PTReturn _get_type;
    void *handle;
    charPReturn _get_name = NULL;
    charPReturn _get_version = NULL;
    char *last_error = NULL;

    std::shared_ptr<Plugin> instance;

public:
    PluginHandler(std::string name);
    ~PluginHandler();

    std::string get_name();
    std::string get_version();
    std::shared_ptr<Plugin> load();
    void unload();
    PluginType get_type();
    bool has_error();
    char *get_error();
    // Use it under your risk... If an error was set maybe something happens.
    void clear_error();
};

std::vector<PluginHandler *> load_plugins(std::string path, std::string extension);

#endif