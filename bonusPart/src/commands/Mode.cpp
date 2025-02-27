#include "../../inc/commands/Mode.hpp"
#include "../../inc/Server.hpp"
#include "../../inc/Channel.hpp"

Mode::Mode() {}

Mode::~Mode() {}

void Mode::execute(Client* client, std::string arguments)
{
    if (client->getState() != REGISTERED)
    {
        client->reply(ERR_NOTREGISTERED());
        return;
    }
    std::istringstream iss(arguments);
    std::string channelName, mode, param;
    iss >> channelName >> mode >> param;

    if (channelName.empty())
    {
        client->reply(ERR_NEEDMOREPARAMS(client->getNickname(), "MODE"));
        return;
    }

    Server* server = Server::getInstance();
    std::map<std::string, Channel*>& channels = server->getChannels();

    if (channels.find(channelName) == channels.end())
    {
        client->reply(ERR_NOSUCHCHANNEL(client->getNickname(), channelName));
        return;
    }

    Channel* channel = channels[channelName];

    if (mode.empty())
    {
        std::string currentModes = channel->getAllModes();
        client->reply(RPL_CHANNELMODEIS(client->getNickname(), channelName, currentModes));
        return;
    }
    if (!channel->isOperator(client))
    {
        client->reply(ERR_CHANOPRIVSNEEDED(client->getNickname(), channelName));
        return;
    }
    if (mode == "+i")
        channel->setInviteOnly(true, client, channel);
    else if (mode == "-i")
        channel->setInviteOnly(false, client, channel);
    else if (mode == "+t")
        channel->setTopicRestricted(true, client, channel);
    else if (mode == "-t")
        channel->setTopicRestricted(false, client, channel);
    else if (mode == "+k")
    {
        if (param.empty())
        {
            client->reply(ERR_NEEDMOREPARAMS(client->getNickname(), mode));
            return;
        }
        if (param[0] == '#')
        {
            client->reply(ERR_PASSWDMISMATCH(client->getNickname()));
            return;
        }
        channel->setPassword(param, client, channel);
        channel->broadcast(NOTICE_PASSWORD_SET(client->getNickname(), channelName), NULL);
    }
    else if (mode == "-k")
    {
        channel->setPassword("", client, channel);
    }
    else if (mode == "+o")
    {
        if (param.empty())
        {
            client->reply(ERR_NEEDMOREPARAMS(client->getNickname(), "MODE +o"));
            return;
        }
        channel->addOperator(param);
        channel->broadcast(NOTICE_OPERATOR_ADDED(client->getNickname(), channelName, param), NULL);
        std::string namesList;
        const std::vector<std::string>& clients = channels[channelName]->getNamesClients();
        for (size_t i = 0; i < clients.size(); ++i) 
        {
            if (!namesList.empty()) 
            {
                namesList += " ";
            }
            if (channels[channelName]->isOperatorByName(clients[i]))
                namesList += "@" + clients[i];
            else
                namesList += clients[i];
        }
        client->reply(RPL_NAMREPLY(client->getNickname(), channelName, namesList));
        client->reply(RPL_ENDOFNAMES(client->getNickname(), channelName));
    }
    else if (mode == "-o")
    {
        if (param.empty())
        {
            client->reply(ERR_NEEDMOREPARAMS(client->getNickname(), "MODE -o"));
            return;
        }
        channel->removeOperator(param);
        channel->broadcast(NOTICE_OPERATOR_REMOVED(client->getNickname(), channelName, param), NULL);
        std::string namesList;
        const std::vector<std::string>& clients = channels[channelName]->getNamesClients();
        for (size_t i = 0; i < clients.size(); ++i) 
        {
            if (!namesList.empty()) 
            {
                namesList += " ";
            }
            if (channels[channelName]->isOperatorByName(clients[i]))
                namesList += "@" + clients[i];
            else
                namesList += clients[i];
        }
        client->reply(RPL_NAMREPLY(client->getNickname(), channelName, namesList));
        client->reply(RPL_ENDOFNAMES(client->getNickname(), channelName));
    }
    else if (mode == "+l")
    {
        if (param.empty())
        {
            client->reply(ERR_NEEDMOREPARAMS(client->getNickname(), "MODE +l"));
            return ;
        }
        size_t newLimit = std::atoi(param.c_str());
        if (newLimit < channel->getUserCount())
        {
            client->reply(ERR_CANNOTSETLIMIT(client->getNickname(), channelName));
            return;
        }
        channel->setUserLimit(newLimit, client, channel);
        channel->broadcast(NOTICE_USER_LIMIT_SET(client->getNickname(), channel->getName(), param), NULL);
    }
    else if (mode == "-l")
        channel->setUserLimit(0, client, channel);
    else
    {
        client->reply(ERR_UNKNOWNMODE(client->getNickname(), mode));
        return;
    }
    channel->broadcast(":" + client->getNickname() + " MODE " + channelName + " " + mode + " " + param, client);
}
