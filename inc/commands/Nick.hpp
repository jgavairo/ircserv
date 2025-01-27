#pragma once

#include "lib.hpp"
#include "Client.hpp"
#include "ACommand.hpp"

class ACommand;

class Nick : public ACommand
{
    public:
        Nick();
        ~Nick();
        std::string _name;
        void use(Client& client, std::string nickname);
};
