#pragma once

#include "lib.hpp"
#include "ACommand.hpp"

class CommandsList
{
    public:
        std::map<std::string, ACommand*> _list;
        CommandsList();
        ~CommandsList();
};

