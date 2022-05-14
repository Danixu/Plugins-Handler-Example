#include <dlfcn.h>
#include <string>
#include "plugin.hpp"
#include <dirent.h>

class PluginHandler
{
    std::shared_ptr<Plugin> (*_load)() = NULL;
    void *handle = NULL;
    char *(*_get_name)() = NULL;
    char *(*_get_version)() = NULL;
    char *last_error = NULL;

    std::shared_ptr<Plugin> instance;

public:
    PluginHandler(std::string name)
    {
        handle = dlopen(name.c_str(), RTLD_LAZY);

        if (!handle || ((last_error = dlerror()) != NULL))
        {
            if (last_error == NULL)
            {
                last_error = (char *)"Handler is empty. Maybe the library file is damaged.";
            }
            fprintf(stderr, "There was an error loading the %s lib:\n%s\n", name.c_str(), last_error);
            return;
        }

        dlerror(); /* Clear any existing error */

        _load = (std::shared_ptr<Plugin>(*)())dlsym(handle, "load");
        if ((last_error = dlerror()) != NULL)
        {
            fprintf(stderr, "Error getting the load symbol in the %s lib:\n%s\n", name.c_str(), last_error);
            return;
        }

        _get_name = (char *(*)())dlsym(handle, "name");
        if ((last_error = dlerror()) != NULL)
        {
            fprintf(stderr, "Error getting the name symbol in the %s lib:\n%s\n", name.c_str(), last_error);
            return;
        }

        _get_version = (char *(*)())dlsym(handle, "version");
        if ((last_error = dlerror()) != NULL)
        {
            fprintf(stderr, "Error getting the version symbol in the %s lib:\n%s\n", name.c_str(), last_error);
            return;
        }
    }

    ~PluginHandler()
    {
        instance.reset();
        if (handle != NULL)
        {
            dlclose(handle);
        }
    }

    std::string get_name()
    {
        return std::string(_get_name());
    }

    std::string get_version()
    {
        return std::string(_get_version());
    }

    std::shared_ptr<Plugin> load()
    {
        if (!instance && _load != NULL)
        {
            instance = _load();
        }

        return instance;
    }

    bool has_error()
    {
        if (last_error != NULL)
        {
            return true;
        }
        return false;
    }

    char *get_error()
    {
        if (last_error == NULL)
        {
            return (char *)'\0';
        }
        else
        {
            return last_error;
        }
    }

    // Use it under your risk... If an error was set maybe something happens.
    void clear_error()
    {
        last_error = NULL;
    }
};

std::vector<PluginHandler> load_plugins(std::string path)
{
    std::vector<PluginHandler> plugins;

    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir(path.c_str())) != NULL)
    {
        while ((ent = readdir(dir)) != NULL)
        {
            if (ent->d_name[0] != '.')
            {
                PluginHandler plugin = PluginHandler(path + std::string(ent->d_name));
                if (!plugin.has_error())
                {
                    plugins.push_back(plugin);
                }
                else
                {
                    fprintf(stderr, "There was an error loading the plugin plugins/%s\n", std::string(ent->d_name).c_str());
                }
            }
        }
        closedir(dir);
    }
    return plugins;
}
