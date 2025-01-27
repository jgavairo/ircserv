#pragma once

#include "lib.hpp"

class ACommand
{
protected:
    /* data */
public:
    std::string _name;
    void use(Client& client, std::string nickname);
    ACommand();
    ~ACommand();
};

