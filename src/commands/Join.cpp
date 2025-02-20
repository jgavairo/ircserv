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
    std::string channelName, keysList;
    iss >> channelName;
    iss >> keysList;
    std::istringstream iss2(channelName);
    std::istringstream iss3(keysList);
    // std::cout << "channels: " << channelName << std::endl;
    // std::cout << "Keys: " << keysList << std::endl;

    Server* server = Server::getInstance();
    std::map<std::string, Channel*>& channels = server->getChannels();
    std::vector<std::string> keys;
    while (std::getline(iss3, keysList, ','))
    {
        keysList.erase(0, keysList.find_first_not_of(" \t"));
        keysList.erase(keysList.find_last_not_of(" \t") + 1);
        keys.push_back(keysList);
    }

    while (std::getline(iss2, channelName, ','))
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

        //verifier si il necessite un mdp, si oui aller checker a lindex associe dans keys
        channels[channelName]->addClient(client);

        std::string namesList;
        const std::vector<std::string>& clients = channels[channelName]->getNamesClients();
        for (size_t i = 0; i < clients.size(); ++i) 
        {
            if (!namesList.empty()) 
            {
                namesList += " ";
            }
            namesList += clients[i];
        }

        client->write(RPL_JOIN(client->getPrefix(), channelName));
        channels[channelName]->broadcast(RPL_JOIN(client->getPrefix(), channelName), client);

        client->reply(RPL_NAMREPLY(client->getNickname(), channelName, namesList));
        client->reply(RPL_ENDOFNAMES(client->getNickname(), channelName));
        if (!channels[channelName]->getTopic().empty())
            client->reply(RPL_TOPIC(client->getNickname(), channelName, channels[channelName]->getTopic()));
        else
            client->reply(RPL_NOTOPIC(client->getNickname(), channelName));
        std::cout << "Client " << client->getFd() << " joined channel: " << channelName << std::endl;

    }

    // /*---------------------------------------------------------------------------------------------------*/
    // //print list channels:
    // /**/std::cout << "List of channels server:\n" << std::endl;
    // /**/
    // /**/for (std::map<std::string, Channel*>::iterator it = channels.begin(); it != channels.end(); ++it)
    // /**/    std::cout << "Channel name: " << it->first << std::endl;
    // /*---------------------------------------------------------------------------------------------------*/
}