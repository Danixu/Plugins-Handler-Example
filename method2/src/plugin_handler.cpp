#include "../include/plugin_handler.hpp"

PluginHandler::PluginHandler(std::string name)
{
// Visual studio fix
#if defined _MSC_VER
	std::wstring namew(name.begin(), name.end());
	if (!(handle = LIBLOAD(namew.c_str())))
#else
	if (!(handle = LIBLOAD(name.c_str())))
#endif
	{
		set_error(dlerror());

		// If the error still null here, then just add a general error text
		if (last_error == NULL)
		{
			char *error_text = new char[54];
			strncpy(error_text, "Handler is empty. Maybe the library file is damaged.\0", sizeof(error_text) - 1);
			set_error(error_text);
		}
		fprintf(stderr, "There was an error loading the %s lib:\n%s\n", name.c_str(), last_error);
		return;
	}

	_load = reinterpret_cast<allocClass>(dlsym(handle, "load"));
	if (_load == NULL)
	{
		set_error(dlerror());
		fprintf(stderr, "Error getting the load symbol in the %s lib:\n%s\n", name.c_str(), last_error);
		return;
	}

	_unload = reinterpret_cast<unloadBoolReturn>(dlsym(handle, "unload"));
	if (_load == NULL)
	{
		set_error(dlerror());
		fprintf(stderr, "Error getting the unload symbol in the %s lib:\n%s\n", name.c_str(), last_error);
		return;
	}

	_get_type = reinterpret_cast<PTReturn>(dlsym(handle, "get_type"));
	if (_get_type == NULL)
	{
		set_error(dlerror());
		fprintf(stderr, "Error getting the get_type symbol in the %s lib:\n%s\n", name.c_str(), last_error);
		return;
	}

	_get_name = reinterpret_cast<charPReturn>(dlsym(handle, "name"));
	if (_get_name == NULL)
	{
		set_error(dlerror());
		fprintf(stderr, "Error getting the name symbol in the %s lib:\n%s\n", name.c_str(), last_error);
		return;
	}

	_get_version = reinterpret_cast<charPReturn>(dlsym(handle, "version"));
	if (_get_version == NULL)
	{
		set_error(dlerror());
		fprintf(stderr, "Error getting the version symbol in the %s lib:\n%s\n", name.c_str(), last_error);
		return;
	}

	_command = reinterpret_cast<commandReturn>(dlsym(handle, "command"));
	if (_command == NULL)
	{
		set_error(dlerror());
		fprintf(stderr, "Error getting the commmand symbol in the %s lib:\n%s\n", name.c_str(), last_error);
		return;
	}
}

PluginHandler::~PluginHandler()
{
	// Last error must be cleared
	if (last_error != NULL)
	{
		delete[] last_error;
	}

	if (command_output != NULL)
	{
		delete[] command_output;
	}

	// If an instance was loaded, delete it
	if (instance != NULL)
	{
		unload();
	}

	// Library must be freed to avoid memory leaks
	if (handle != NULL)
	{
		dlclose(handle);
		handle = NULL;
	}
}

void PluginHandler::load()
{
	if (instance == NULL)
	{
		if (_load != NULL)
		{

			instance = _load();
			return;
		}
	}

	return;
}

bool PluginHandler::unload()
{
	if (instance != NULL)
	{
		if (_unload != NULL)
		{

			_unload(instance);
			instance = NULL;
			return true;
		}
	}

	return false;
}

std::string PluginHandler::get_name()
{
	return std::string(_get_name());
}

std::string PluginHandler::get_version()
{
	return std::string(_get_version());
}

PluginType PluginHandler::get_type()
{
	return _get_type();
}

std::string PluginHandler::command(char *command, char *options)
{
	if (command_output != NULL)
	{
		delete[] command_output;
	}
	command_output = _command(instance, command, options);
	return std::string(command_output);
}

bool PluginHandler::has_error()
{
	if (last_error != NULL)
	{
		return true;
	}
	return false;
}

char *PluginHandler::get_error()
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

void PluginHandler::set_error(char *error_text)
{
	// First we will clear the last error to free the memory
	clear_error();

	// Just set the new error
	last_error = error_text;
}

// Use it under your risk... If an error was set maybe something happens.
void PluginHandler::clear_error()
{
	if (last_error != NULL)
	{
		delete[] last_error;
	}

	last_error = NULL;
}

// Plugin loader which will return a bunch of pointers to the plugins
std::vector<std::shared_ptr<PluginHandler>> load_plugins(std::string path, std::string extension, PluginType type)
{
	std::vector<std::shared_ptr<PluginHandler>> plugins;

	for (auto &p : std::filesystem::recursive_directory_iterator(path))
	{
		if (p.path().extension() == extension)
		{
			std::shared_ptr<PluginHandler> plugin = std::make_shared<PluginHandler>(p.path().string());
			if (!plugin->has_error())
			{
				// PluginType 0 means read all, or can be filtered using the PluginType
				if (type == (PluginType)0 || plugin->get_type() == type)
				{
					// If the plugin type match, add it to the vector
					plugins.push_back(plugin);
				}
				// Non added plugins will be freed automatically
			}
		}
	}

	return plugins;
}
