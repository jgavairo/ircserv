#include "../../inc/commands/Topic.hpp"
#include "../../inc/Channel.hpp"
#include "../../inc/Server.hpp"

Topic::Topic() {}

Topic::~Topic() {}

void Topic::execute(Client* client, std::string arguments)
{
    if (arguments.empty())
    {
        client->reply(ERR_NEEDMOREPARAMS(client->getNickname(), "TOPIC"));
        return ;
    }
    std::istringstream iss(arguments);
    std::string channel, newTopic;

    iss >> channel;
    std::getline(iss, newTopic);

    newTopic.erase(0, newTopic.find_first_not_of(" :"));
    std::cout << "Channel is: [" << channel << "]" << std::endl;
    std::cout << "new Topic is: [" << newTopic << "]" << std::endl;

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
        if (!client->isOperator())
        {
            client->reply(ERR_CHANOPRIVSNEEDED(client->getNickname(), channel));
            return ;
        }
        channelTarget->setTopic(newTopic);
        return ;
    }
    else
    {
        client->reply(ERR_NOSUCHCHANNEL(client->getNickname(), channel));
        return ;
    }
}