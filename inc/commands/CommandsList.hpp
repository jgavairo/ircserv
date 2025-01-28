#pragma once

#include "lib.hpp"
#include "ACommand.hpp"

class CommandsList
{
    private:
        //peut etre passer la liste en private
    public:
        std::map<std::string, ACommand*> _list;
        CommandsList();
        ~CommandsList();
};

