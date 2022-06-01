#pragma once

/*

  This header file is the virtual plugin definition which will be used in derivated plugins and main program

*/

#include "export.h"
#include <string>
#include <memory>

#include <iostream>
#include <cstring>

#ifndef _PLUGIN_HPP_H_
#define _PLUGIN_HPP_H_
enum PluginType
{
    PTAll = 0,
    PTInput,
    PTOutput
};

class Plugin1
{
public:
    Plugin1();
    ~Plugin1();
    std::string command(std::string command, std::string options);
};

#endif // _PLUGIN_HPP_H_