#pragma once

#include "ACommand.hpp"

class Quit : public ACommand
{
public:
    Quit();
    ~Quit();
    void execute(Client* client, std::string arguments);
};