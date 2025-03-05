#include "../../inc/commands/Part.hpp"
#include "../../inc/Server.hpp"
#include "../../inc/Channel.hpp"

Part::Part() {}

Part::~Part() {}

void Part::execute(Client* client, std::string arguments)
{
    if (client->getState() == NOT_REGISTERED)
    {
        client->reply(ERR_NOTREGISTERED());
        return;
    }
    Server* server = Server::getInstance();
    std::map<std::string, Channel*>& channels = server->getChannels();

    std::string channelName, partMessage, target;

    size_t pos = arguments.find(':');
    if (pos != std::string::npos)
    {
        partMessage = arguments.substr(pos, arguments.size());
        partMessage.erase(0, channelName.find_first_not_of(" :"));
    }
    channelName = arguments.substr(0, pos - 1);
    std::istringstream iss(channelName);
    std::cout << "channelName: " << channelName << std::endl;
    
    if (channelName.empty())
    {
        client->reply(ERR_NEEDMOREPARAMS(client->getNickname(), std::string("JOIN")));
        return;
    }

    while (std::getline(iss, channelName, ','))
    {
        channelName.erase(0, channelName.find_first_not_of(" \t"));
        channelName.erase(channelName.find_last_not_of(" \t") + 1);

        if (channelName.empty())
        {
            client->reply(ERR_NEEDMOREPARAMS(client->getNickname(), std::string("JOIN")));
            return;
        }

        if (channelName[0] != '#' || channels.find(channelName) == channels.end() || client->_channels.find(channelName) == client->_channels.end())
        {
            client->reply(ERR_NOSUCHCHANNEL(client->getNickname(), channelName));
            return;
        }
        channels[channelName]->broadcast(PART_MESSAGE(client->getPrefix(), channelName, partMessage), NULL);
        channels[channelName]->removeClient(client);
        if (channels[channelName]->isOperator(client))
            channels[channelName]->removeOperator(client->getNickname());
    }
}