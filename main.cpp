/*
 *
 * Main application which will load the plugins dinamically
 *
 */

#include <vector>
#include "plugin_handler.hpp"

#ifdef _WIN32
#define EXT ".dll"
#else
#define EXT ".so"
#endif

int main()
{
    auto plugins = load_plugins("plugins/", EXT);
    for (auto ph : plugins)
    {
        fprintf(stderr, "\n\nLoading plugin...\n");
        auto plugin = ph.load();
        if (plugin == NULL)
        {
            fprintf(stderr, "The plugin is not loaded correctly\n");
            continue;
        }
        fprintf(stderr, "Plugin loaded\n");
        fprintf(stderr, "Auto loaded plugin: %s, version: %s\n", ph.get_name().c_str(), ph.get_version().c_str());
        fprintf(stderr, "Running plugins command method:\n");
        fprintf(stderr, "%s\n", plugin->command("Command here", "options here").c_str());
    }
    return 0;
}