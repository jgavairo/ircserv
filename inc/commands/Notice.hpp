#pragma once

#include "ACommand.hpp"

class Notice : public ACommand
{
    public:
        Notice();
        ~Notice();
        void execute(Client* client, std::string arguments);
};