#include <string>
#include "plugin.hpp"
#include <filesystem>
#include <memory>

class PluginHandler
{
    using allocClass = Plugin *(*)();
    using deleteClass = void (*)(Plugin *);
    using charPReturn = char *(*)();

    allocClass _load = NULL;
    deleteClass _unload = NULL;
    void *handle;
    charPReturn _get_name = NULL;
    charPReturn _get_version = NULL;
    char *last_error = NULL;

    std::shared_ptr<Plugin> instance;

public:
    PluginHandler(std::string name)
    {
        if (!(handle = LIBLOAD(name.c_str())))
        {
            last_error = dlerror();

            // If the error still null here, then just add a general error text
            if (last_error == NULL)
            {
                last_error = (char *)"Handler is empty. Maybe the library file is damaged.";
            }
            fprintf(stderr, "There was an error loading the %s lib:\n%s\n", name.c_str(), last_error);
            return;
        }

        _load = reinterpret_cast<allocClass>(dlsym(handle, "load"));
        if ((last_error = dlerror()) != NULL)
        {
            fprintf(stderr, "Error getting the load symbol in the %s lib:\n%s\n", name.c_str(), last_error);
            return;
        }

        _unload = reinterpret_cast<deleteClass>(dlsym(handle, "unload"));
        if ((last_error = dlerror()) != NULL)
        {
            fprintf(stderr, "Error getting the load symbol in the %s lib:\n%s\n", name.c_str(), last_error);
            return;
        }

        _get_name = reinterpret_cast<charPReturn>(dlsym(handle, "name"));
        if ((last_error = dlerror()) != NULL)
        {
            fprintf(stderr, "Error getting the name symbol in the %s lib:\n%s\n", name.c_str(), last_error);
            return;
        }

        _get_version = reinterpret_cast<charPReturn>(dlsym(handle, "version"));
        if ((last_error = dlerror()) != NULL)
        {
            fprintf(stderr, "Error getting the version symbol in the %s lib:\n%s\n", name.c_str(), last_error);
            return;
        }
    }

    ~PluginHandler()
    {
        if (last_error != NULL)
        {
            delete[] last_error;
        }

        /*
        if (handle != NULL)
        {
            dlclose(handle);
            handle = NULL;
        }
        */
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
        if (_load != NULL)
        {
            instance = std::shared_ptr<Plugin>(_load(), _unload);
            return instance;
        }

        return NULL;
    }

    void unload()
    {
        if (_unload != NULL)
        {
            _unload(instance.get());
        }
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
        delete[] last_error;
        last_error = NULL;
    }
};

std::vector<PluginHandler *> load_plugins(std::string path, std::string extension)
{
    std::vector<PluginHandler *> plugins;

    for (auto &p : std::filesystem::recursive_directory_iterator(path))
    {
        if (p.path().extension() == extension)
        {
            PluginHandler *plugin = new PluginHandler(p.path().string());
            if (!plugin->has_error())
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
