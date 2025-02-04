#include "../../inc/commands/Join.hpp"
#include "../../inc/Channel.hpp"
#include "../../inc/Server.hpp"

Join::Join() {}

Join::~Join() {}

void Join::execute(Client* client, std::string arguments)
{
    if (client->getState() != REGISTERED)
    {
        client->reply(ERR_NOTREGISTERED());
        return ;
    }

    std::istringstream iss(arguments);
    std::string channelName;
    iss >> channelName;

    if (channelName.empty())
    {
        client->reply(ERR_NEEDMOREPARAMS(client->getNickname(), std::string("JOIN")));
        return ;
    }

    if (channelName[0] != '#')
    {
        client->reply(ERR_NOSUCHCHANNEL(client->getNickname(), channelName));
		return ;
    }
    
    Server* server = Server::getInstance();

    std::map<std::string, Channel*>& channels = server->getChannels();

    if (channels.find(channelName) == channels.end())
        channels[channelName] = new Channel(channelName);

    channels[channelName]->addClient(client);

    client->reply(RPL_JOIN(client->getNickname(), channelName));

    std::cout << "Client " << client->getFd() << " joined channel: " << channelName << std::endl;

    std::cout << "List of channels:\n" << std::endl;

    for (std::map<std::string, Channel*>::iterator it = channels.begin(); it != channels.end(); ++it)
        std::cout << "Channel name: " << it->first << std::endl;
}