#include <cstring>
#include <string>
#include <vector>
#include <filesystem>
#include <memory>

#include "export.h"

#ifndef _PLUGIN_HANDLER_H_
#define _PLUGIN_HANDLER_H_

enum PluginType
{
    PTAll = 0,
    PTInput,
    PTOutput
};

class PluginHandler
{
private:
    using allocClass = void *(*)();
    using charPReturn = char *(*)();
    using unloadBoolReturn = bool (*)(void *);
    using PTReturn = PluginType (*)();
    using commandReturn = char *(*)(void *, char *, char *);

    allocClass _load = NULL;
    unloadBoolReturn _unload = NULL;
    PTReturn _get_type;
    charPReturn _get_name = NULL;
    charPReturn _get_version = NULL;
    commandReturn _command = NULL;
    char *last_error = NULL;

    void *handle = NULL;
    void *instance = NULL;

    char *command_output = NULL;

    void set_error(char *error_text);

public:
    PluginHandler(std::string name);
    ~PluginHandler();

    std::string get_name();
    std::string get_version();
    void load();
    bool unload();
    PluginType get_type();
    std::string command(char *command, char *options);
    bool has_error();
    char *get_error();
    void clear_error();
};

std::vector<std::shared_ptr<PluginHandler>> load_plugins(std::string path, std::string extension, PluginType type = (PluginType)0);
#endif // _PLUGIN_HANDLER_H_