#pragma once

#include "ACommand.hpp"

class List : public ACommand
{
    public:
        List();
        ~List();
        void execute(Client* client, std::string arguments);
};