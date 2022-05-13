#include <dlfcn.h>
#include "plugin.hpp"
#include <iostream>

class PluginHandler
{
    std::shared_ptr<Plugin> (*_load)() = NULL;
    void *handle = NULL;
    char *(*_get_name)() = NULL;
    char *(*_get_version)() = NULL;

    char *error = NULL;

    std::shared_ptr<Plugin> instance;

public:
    PluginHandler(std::string name)
    {
        handle = dlopen(name.c_str(), RTLD_LAZY);

        if (!handle || ((error = dlerror()) != NULL))
        {
            if (error == NULL)
            {
                error = (char *)"Handler is empty. Maybe the library file is damaged.";
            }
            fprintf(stderr, "There was an error loading the %s lib:\n%s\n", name.c_str(), error);
            return;
        }

        dlerror(); /* Clear any existing error */

        _load = (std::shared_ptr<Plugin>(*)())dlsym(handle, "load");
        if ((error = dlerror()) != NULL)
        {
            fprintf(stderr, "Error getting the load symbol in the %s lib:\n%s\n", name.c_str(), error);
            return;
        }

        _get_name = (char *(*)())dlsym(handle, "name");
        if ((error = dlerror()) != NULL)
        {
            fprintf(stderr, "Error getting the name symbol in the %s lib:\n%s\n", name.c_str(), error);
            return;
        }

        _get_version = (char *(*)())dlsym(handle, "version");
        if ((error = dlerror()) != NULL)
        {
            fprintf(stderr, "Error getting the version symbol in the %s lib:\n%s\n", name.c_str(), error);
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
};