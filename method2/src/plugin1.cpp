#include "plugin1.hpp"

Plugin1::Plugin1() {}
Plugin1::~Plugin1(){};

std::string Plugin1::command(std::string command, std::string options)
{
    return command + " " + options;
}

extern "C"
{
    void SHARED_EXPORT *load()
    {
        Plugin1 *plugin = new Plugin1();
        return (void *)plugin;
    }

    void SHARED_EXPORT freechararray(char *ptr)
    {
        delete[] ptr;
    }

    void SHARED_EXPORT unload(void *ptr)
    {
        delete (Plugin1 *)ptr;
    }

    int SHARED_EXPORT get_type()
    {
        return PTInput;
    }

    const char SHARED_EXPORT *name()
    {
        return "Plugin1";
    }

    const char SHARED_EXPORT *version()
    {
        return "0.0.1";
    }

    char SHARED_EXPORT *command(void *ptr, char *command, char *options)
    {
        Plugin1 *plugin = (Plugin1 *)ptr;
        std::string str_output = plugin->command(std::string(command), std::string(options));
        char *output = new char[str_output.length() + 1];
        strcpy(output, str_output.c_str());
        return output;
    }
}