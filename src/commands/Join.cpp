#include "../../inc/commands/Join.hpp"
#include "../../inc/Channel.hpp"
#include "../../inc/Server.hpp"

Join::Join() {}

Join::~Join() {}

void Join::execute(Client* client, std::string arguments)
{
    //Verification des droits (client authentifie)
    if (client->getState() != REGISTERED)
    {
        client->reply(ERR_NOTREGISTERED());
        return ;
    }
    //Split des noms de channels par virgule (RFC 1459)
    std::istringstream iss(arguments);
    std::string channelName;
    Server* server = Server::getInstance();
    std::map<std::string, Channel*>& channels = server->getChannels();

    while (std::getline(iss, channelName, ','))
    {
        //Supprimer les espaces éventuels autour du nom du canal
        channelName.erase(0, channelName.find_first_not_of(" \t"));
        channelName.erase(channelName.find_last_not_of(" \t") + 1);

        if (channelName.empty())
        {
            client->reply(ERR_NEEDMOREPARAMS(client->getNickname(), std::string("JOIN")));
            return;
        }

        if (channelName[0] != '#')
        {
            client->reply(ERR_NOSUCHCHANNEL(client->getNickname(), channelName));
            return;
        }

        if (channels.find(channelName) == channels.end())
            channels[channelName] = new Channel(channelName);

        channels[channelName]->addClient(client);


        client->write(RPL_JOIN(client->getPrefix(), channelName));
        channels[channelName]->broadcast(RPL_JOIN(client->getPrefix(), channelName), client);

        std::cout << "Client " << client->getFd() << " joined channel: " << channelName << std::endl;

    }
    // /*---------------------------------------------------------------------------------------------------*/
    // //print list channels:
    // /**/std::cout << "List of channels:\n" << std::endl;
    // /**/
    // /**/for (std::map<std::string, Channel*>::iterator it = channels.begin(); it != channels.end(); ++it)
    // /**/    std::cout << "Channel name: " << it->first << std::endl;
    // /*---------------------------------------------------------------------------------------------------*/
}