#include "../../inc/commands/Notice.hpp"
#include "../../inc/Channel.hpp"
#include "../../inc/Server.hpp"

Notice::Notice() {}

Notice::~Notice() {}

void Notice::execute(Client* client, std::string arguments) 
{
    if (client->getState() != REGISTERED)
    {
        client->reply(ERR_NOTREGISTERED());
        return ;
    }
    std::istringstream iss(arguments);
    std::string target;
    std::string message;

    iss >> target;
    std::getline(iss, message);

    Server* server = Server::getInstance();
    std::map<int, Client*> clientsList = server->getClients();
    std::map<std::string, Channel*> channelsList = server->getChannels();
    if (target.empty() || message.empty()) {
        client->reply(ERR_NEEDMOREPARAMS(client->getNickname(), "NOTICE"));
        return;
    }

    std::string noticeMessage = ":" + client->getPrefix() + " NOTICE " + target + " :" + message;

    if (target[0] == '#') 
    {
        if (channelsList.find(target) != channelsList.end()) 
        {
            if (client->_channels.find(target) == client->_channels.end()) 
            {
                client->reply(ERR_NOTONCHANNEL(client->getNickname(), target));
                return;
            }
            channelsList[target]->broadcast(noticeMessage, client);
        }
        else
            client->reply(ERR_NOSUCHCHANNEL(client->getNickname(), target));
    } 
    else 
    {
        bool found = false;
        for (std::map<int, Client*>::iterator it = clientsList.begin(); it != clientsList.end(); ++it)
        {
            if (target == it->second->getNickname())
            {
                it->second->write(noticeMessage);
                found = true;
                break ;
            }
        }
        if (found == false)
            client->reply(ERR_NOSUCHNICK(client->getNickname(), target));
    }
}
