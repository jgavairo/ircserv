#include "../../inc/commands/Kick.hpp"
#include "../../inc/Channel.hpp"
#include "../../inc/Server.hpp"

Kick::Kick() {}

Kick::~Kick() {}

void Kick::execute(Client* client, std::string arguments)
{
    std::istringstream iss(arguments);
    std::string channel, target, reason;
    Server* server = Server::getInstance();
    std::map<std::string, Channel*> serverChannels = server->getChannels();
    std::map<int, Client*> serverClients = server->getClients();
    size_t pos = arguments.find(":") + 1;
    if (pos != std::string::npos)
        reason = arguments.substr(pos, arguments.size());
    
    iss >> channel;
    iss >> target;

    std::cout << "channel : [" << channel << "]" << std::endl;
    std::cout << "target : [" << target << "]" << std::endl;
    std::cout << "reason : [" << reason << "]" << std::endl;

    if (channel.empty() || target.empty())
    {
        client->reply(ERR_NEEDMOREPARAMS(client->getNickname(), "KICK"));
        return ;
    }
    if (serverChannels.find(channel) == serverChannels.end())
    {
        client->reply(ERR_NOSUCHCHANNEL(client->getNickname(), channel));
        return ;
    }
    if (!client->searchChannel(channel))
    {
        client->reply(ERR_NOTONCHANNEL(client->getNickname(), channel));
        return ;
    }
    if (!serverChannels[channel]->isOperator(client))
    {
        client->reply(ERR_CHANOPRIVSNEEDED(client->getNickname(), channel));
        return ;
    }
    for (std::map<int, Client*>::iterator it = serverClients.begin(); it != serverClients.end(); ++it)
    {
        if (it->second->getNickname() == target)
        {
            if (!it->second->searchChannel(channel))
            {
                client->reply(ERR_USERNOTINCHANNEL(client->getNickname(), target, channel));
                return ;
            }
            if (serverChannels[channel]->isInvited(it->second))
            {
                serverChannels[channel]->removeInvite(target);
            }
            serverChannels[channel]->broadcast(RPL_KICK(client->getNickname(), channel, target, reason), NULL);
            serverChannels[channel]->removeClient(it->second);
            return;
        }
    }
    client->reply(ERR_NOSUCHNICK(client->getNickname(), target));
}