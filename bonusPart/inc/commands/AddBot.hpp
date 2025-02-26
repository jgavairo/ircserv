#pragma once

#include "ACommand.hpp"

class Channel;

class AddBot : public ACommand
{
    public:
        AddBot();
        ~AddBot();
        void execute(Client* client, std::string arguments);
};