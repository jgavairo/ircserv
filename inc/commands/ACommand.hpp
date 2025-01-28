#pragma once

#include "lib.hpp"
#include "../Client.hpp"

class ACommand
{
protected:
    ACommand();
public:
    virtual ~ACommand();
    virtual void execute(Client* client, std::string arguments) = 0;
};

class Nick : public ACommand
{
    public:
        Nick();
        ~Nick();
        std::string _name;
        void execute(Client* client, std::string arguments);
};