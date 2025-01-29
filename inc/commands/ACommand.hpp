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
    std::vector<std::string> splitArguments(std::string args);
};

class Nick : public ACommand
{
    public:
        Nick();
        ~Nick();
        void execute(Client* client, std::string arguments);
};

class User : public ACommand
{
    public:
        User();
        ~User();
        void execute(Client* client, std::string arguments);
};