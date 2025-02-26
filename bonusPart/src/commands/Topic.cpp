#include "../../inc/commands/Topic.hpp"
#include "../../inc/Channel.hpp"
#include "../../inc/Server.hpp"

Topic::Topic() {}

Topic::~Topic() {}

void Topic::execute(Client* client, std::string arguments)
{
    if (client->getState() == NOT_REGISTERED)
    {
        client->reply(ERR_NOTREGISTERED());
        return;
    }
    if (arguments.empty())
    {
        client->reply(ERR_NEEDMOREPARAMS(client->getNickname(), "TOPIC"));
        return ;
    }
    std::istringstream iss(arguments);
    std::string channel, newTopic;
    Server* server = Server::getInstance();
    iss >> channel;
    std::getline(iss, newTopic);

    newTopic.erase(0, newTopic.find_first_not_of(" :"));
    std::map<int, Client*> serverClients = server->getClients();
    Channel* channelTarget = client->searchChannel(channel);
    if (channelTarget && newTopic.empty())
    {
        if (channelTarget->getTopic().empty())
        {
            client->reply(RPL_NOTOPIC(client->getNickname(), channel));
            return ;
        }
        else
        {
            client->reply(RPL_TOPIC(client->getNickname(), channelTarget->getName(), channelTarget->getTopic()));
            return ;
        }
    }
    else if (channelTarget && !newTopic.empty())
    {
        if (channelTarget->isTopicRestricted() && !channelTarget->isOperator(client))
        {
            client->reply(ERR_CHANOPRIVSNEEDED(client->getNickname(), channel));
            return ;
        }
        channelTarget->setTopic(newTopic);
        for (std::map<int, Client*>::iterator it = serverClients.begin(); it != serverClients.end(); ++it)
        {
            if (it->second->searchChannel(channel))
                it->second->reply(RPL_TOPIC(client->getNickname(), channelTarget->getName(), channelTarget->getTopic()));
        }
        return ;
    }
    else
    {
        client->reply(ERR_NOSUCHCHANNEL(client->getNickname(), channel));
        return ;
    }
}