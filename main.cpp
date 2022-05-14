/*
 *
 * Main application which will load the plugins dinamically
 *
 */

#include <vector>
#include <iostream>
#include <dirent.h>
#include "plugin_handler.hpp"

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

int main()
{
    auto plugins = load_plugins("plugins/");
    for (auto ph : plugins)
    {
        std::cerr << "Loading plugin..." << std::endl;
        auto plugin = ph.load();
        if (plugin == NULL)
        {
            fprintf(stderr, "The plugin is not loaded correctly\n");
            continue;
        }
        std::cerr << "Plugin loaded" << std::endl;
        std::cerr << "Auto loaded plugin: " << ph.get_name() << ", version: " << ph.get_version() << std::endl;
        std::cerr << "Running plugins command method: " << std::endl;
        std::cerr << plugin->command("Command here", "options here") << std::endl;
    }
    return 0;
}