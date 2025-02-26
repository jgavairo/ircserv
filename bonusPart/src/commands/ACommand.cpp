#include "../../inc/commands/ACommand.hpp"

ACommand::ACommand() {} 

ACommand::~ACommand() {}

std::vector<std::string> ACommand::splitArguments(std::string args)
{
    std::vector<std::string> arguments;
    std::string arg;
    std::stringstream iss(args);

    while (iss >> arg)
    {
        if (!arg.empty())
            arguments.push_back(arg);
    }
    return arguments;
}