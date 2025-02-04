#include "../../inc/commands/Nick.hpp"

Nick::Nick() {}

Nick::~Nick() {}

void Nick::execute(Client* client, std::string arguments)
{
    std::istringstream iss(arguments);
    std::string new_nickname;

    iss >> new_nickname;

    client->setNickname(new_nickname);
    if (client->getState() == WAITING_FOR_NICK)
    {
        client->setState(REGISTERED);
        client->reply(RPL_WELCOME(client->getNickname()));
    }
    else if (client->getState() == NOT_REGISTERED)
        client->setState(WAITING_FOR_USER);
    std::cout << "----Command 'NICK' has been executed on client " << client->getFd() << ". her nickname is now " << client->getNickname() << "----" << std::endl;
}