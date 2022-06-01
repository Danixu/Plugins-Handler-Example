/*
 *
 * Main application which will load the plugins dinamically
 *
 */

#include <vector>
#include "../include/plugin_handler.hpp"

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
        fprintf(stderr, "Loading plugin...\n");
        ph->load();
        if (ph->has_error())
        {
            fprintf(stderr, "The plugin was not loaded correctly\n");
            continue;
        }
        fprintf(stderr, "Plugin loaded\n");
        fprintf(stderr, "Auto loaded plugin: %s, version: %s, type: %d\n", ph->get_name().c_str(), ph->get_version().c_str(), ph->get_type());
        fprintf(stderr, "Running plugins command method:\n");

        const char *command = "Command here";
        const char *options = "Options here";
        fprintf(stderr, "%s\n", ph->command((char *)command, (char *)options).c_str());
    }

    return 0;
}