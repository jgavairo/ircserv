#include "../../inc/commands/Nick.hpp"
#include "../../inc/Server.hpp"
#include "../../inc/Channel.hpp"

Nick::Nick() {}

Nick::~Nick() {}

void Nick::execute(Client* client, std::string arguments)
{
    if (!client->isAuthenticated())
    {
        client->reply(ERR_NOTREGISTERED());
        return ;
    }
    if (arguments.empty())
    {
        client->reply(ERR_NONICKNAMEGIVEN(client->getNickname()));
        return ;
    }
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
    if (client->getState() == WAITING_FOR_NICK)
    {
        client->setState(REGISTERED);
    }
    else if (client->getState() == NOT_REGISTERED)
        client->setState(WAITING_FOR_USER);
    if (client->getState() == REGISTERED)
        client->reply(RPL_WELCOME(new_nickname));

    for (std::map<std::string, Channel*>::iterator it = client->_channels.begin(); it != client->_channels.end(); ++it)
    {
        it->second->broadcast(RPL_NICK(client->getPrefix(), new_nickname), client);
        std::string noticeMessage = ":" + client->getPrefix() + " NOTICE " + it->second->getName() + " :" + old_nick + " is now known as " + new_nickname;
        it->second->broadcast(noticeMessage, client);
    }

    client->write(RPL_NICK(client->getPrefix(), new_nickname));
    server->updateNickInChannels(old_nick, new_nickname);
    client->setNickname(new_nickname);
}