#ifndef ACOMMAND_HPP
#define ACOMMAND_HPP

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


#endif