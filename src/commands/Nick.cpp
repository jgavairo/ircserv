#include "../../inc/commands/Nick.hpp"
#include "../../inc/Server.hpp"
#include "../../inc/Channel.hpp"

Nick::Nick() {}

Nick::~Nick() {}

#include <cctype>
#include <string>

bool isValidNickname(const std::string& nickname) {
    if (nickname.length() > 9 || !isalpha(nickname[0])) {
        return false;
    }
    for (size_t i = 1; i < nickname.length(); ++i) {
        char c = nickname[i];
        if (!isalpha(c) && !isalnum(c) && c != '-' && c != '_') {
            return false;
        }
    }
    return true;
}

void Nick::execute(Client* client, std::string arguments)
{
    if (!client->isAuthenticated())
    {
        client->reply(ERR_NOTREGISTERED());
        return ;
    }
    std::istringstream iss(arguments);
    std::string new_nickname;
    iss >> new_nickname;
    if (new_nickname.empty())
    {
        client->reply(ERR_NONICKNAMEGIVEN(client->getNickname()));
        return ;
    }
    if (!isValidNickname(new_nickname))
    {
        client->reply(ERR_ERRONEUSNICKNAME(arguments));
        return ;
    }

    Server* server = Server::getInstance();
    std::map<int, Client*>& clientsList = server->getClients();
    for (std::map<int, Client*>::iterator it = clientsList.begin(); it != clientsList.end(); ++it)
    {
        if (it->second->getNickname() == new_nickname)
        {
            std::cerr << "Nickname " << new_nickname << " has already used by client" << it->first << std::endl;
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
