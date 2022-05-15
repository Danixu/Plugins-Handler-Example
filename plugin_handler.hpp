#include <string>
#include "plugin.hpp"
#include <filesystem>

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
        handle = LIBLOAD(name.c_str());
        if (!handle || ((last_error = dlerror()) != NULL))
        {
            // Maybe the last_error variable is NULL because the handler is empty directly.
            // In that case, try to return the error again
            if (last_error == NULL)
            {
                last_error = dlerror();
            }

            // If the error still null here, then just add a general error text
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
            fprintf(stderr, "Iniatilizing the class %d\n", _load);
            instance = _load();
            fprintf(stderr, "Initialized...\n");
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

std::vector<PluginHandler> load_plugins(std::string path, std::string extension)
{
    std::vector<PluginHandler> plugins;

    for (auto &p : std::filesystem::recursive_directory_iterator(path))
    {
        if (p.path().extension() == extension)
        {
            PluginHandler plugin = PluginHandler(p.path().string());
            if (!plugin.has_error())
            {
                plugins.push_back(plugin);
            }
            else
            {
                fprintf(stderr, "There was an error loading the plugin %s\n", p.path().string().c_str());
            }
        }
    }

    return plugins;
}
