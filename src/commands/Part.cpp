#include "../../inc/commands/Part.hpp"
#include "../../inc/Server.hpp"
#include "../../inc/Channel.hpp"

Part::Part() {}

Part::~Part() {}

void Part::execute(Client* client, std::string arguments)
{
    Server* server = Server::getInstance();
    std::map<std::string, Channel*>& channels = server->getChannels();
    /*---------------------------------------------------------------------------------------------------*/
    //print list channels:
    /**/std::cout << "List of channels server:\n" << std::endl;
    /**/
    /**/for (std::map<std::string, Channel*>::iterator it = channels.begin(); it != channels.end(); ++it)
    /**/    std::cout << "Channel name: " << it->first << std::endl;
    /*---------------------------------------------------------------------------------------------------*/


    /*---------------------------------------------------------------------------------------------------*/
    //print list channels:
    /**/std::cout << "List of channels Client:\n" << std::endl;
    /**/
    /**/for (std::map<std::string, Channel*>::iterator it = client->_channels.begin(); it != client->_channels.end(); ++it)
    /**/    std::cout << "Channel name: " << it->first << std::endl;
    /*---------------------------------------------------------------------------------------------------*/



    //Split des noms de channels par virgule (RFC 1459)
    std::string channelName, partMessage;

    size_t pos = arguments.find(':');
    if (pos != std::string::npos)
    {
        channelName = arguments.substr(0, pos - 1);
        partMessage = arguments.substr(pos, arguments.size());
    }
    std::istringstream iss(channelName);

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

        if (channelName[0] != '#' || channels.find(channelName) == channels.end() || client->_channels.find(channelName) == client->_channels.end())
        {
            client->reply(ERR_NOSUCHCHANNEL(client->getNickname(), channelName));
            return;
        }

        channels[channelName]->removeClient(client);


        //SEGFAULT SI NO COMMENTARY, IL FAUT AJOUTER LA CONDITION SI LE CHANNEL EXISTE TOUJOURS
        // std::string namesList;
        // const std::vector<std::string>& clients = channels[channelName]->getNamesClients();
        // for (size_t i = 0; i < clients.size(); ++i) 
        // {
        //     if (!namesList.empty()) 
        //     {
        //         namesList += " ";
        //     }
        //     namesList += clients[i];
        // }
        // client->reply(RPL_NAMREPLY(client->getNickname(), channelName, namesList));
        // client->reply(RPL_ENDOFNAMES(client->getNickname(), channelName));

        
    }
}