#include "../../inc/commands/Nick.hpp"
#include "../../inc/Server.hpp"

Nick::Nick() {}

Nick::~Nick() {}

void Nick::execute(Client* client, std::string arguments)
{
    std::istringstream iss(arguments);
    std::string new_nickname;
    iss >> new_nickname;

    Server* server = Server::getInstance();
    std::map<int, Client*>& clientsList = server->getClients();
    for (std::map<int, Client*>::iterator it = clientsList.begin(); it != clientsList.end(); ++it)
    {
        if (it->second->getNickname() == new_nickname)
        {
            std::cerr << "Nickname " << new_nickname << "has already used by client" << it->first << std::endl;
            client->reply(ERR_NICKNAMEINUSE(new_nickname));
            return ;
        }
    }

    std::string old_nick = client->getNickname();
    client->setNickname(new_nickname);
    if (client->getState() == REGISTERED)
        client->write(RPL_NICK(client->getPrefix(), new_nickname));
    if (client->getState() == WAITING_FOR_NICK)
    {
        client->setState(REGISTERED);
        client->reply(RPL_WELCOME(client->getNickname()));
    }
    else if (client->getState() == NOT_REGISTERED)
        client->setState(WAITING_FOR_USER);
    std::cout << "----Command 'NICK' has been executed on client " << client->getFd() << ". her nickname is now " << client->getNickname() << "----" << std::endl;
}