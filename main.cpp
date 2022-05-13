/*
 *
 * Main application which will load the plugins dinamically
 *
 */

#include <vector>
#include <iostream>
#include <dirent.h>
#include "plugin_handler.hpp"

std::vector<PluginHandler> load_plugins()
{
    std::vector<PluginHandler> plugins;

    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir("plugins/")) != NULL)
    {
        while ((ent = readdir(dir)) != NULL)
        {
            if (ent->d_name[0] != '.')
            {
                plugins.push_back(PluginHandler("plugins/" + std::string(ent->d_name)));
            }
        }
        closedir(dir);
    }
    return plugins;
}

int main()
{
    auto plugins = load_plugins();
    for (auto ph : plugins)
    {
        std::cerr << "Loading plugin..." << std::endl;
        auto plugin = ph.load();
        std::cerr << "Plugin loaded" << std::endl;
        std::cerr << "Auto loaded plugin: " << ph.get_name() << ", version: " << ph.get_version() << std::endl;
        std::cerr << "Running plugins command method: " << std::endl;
        std::cerr << plugin->command("Command here", "options here") << std::endl;
    }
    return 0;
}