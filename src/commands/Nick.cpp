#include "../../inc/commands/ACommand.hpp"

Nick::Nick() {}

Nick::~Nick() {}

void Nick::execute(Client* client, std::string arguments)
{
    std::istringstream iss(arguments);
    std::string new_nickname;

    iss >> new_nickname;

    client->setNickname(new_nickname);
    std::cout << "----Command 'NICK' has been executed on client " << client->getFd() << ". her nickname is now " << client->getNickname() << "----" << std::endl;
}