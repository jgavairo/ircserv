#include "../../inc/commands/Notice.hpp"
#include "../../inc/Channel.hpp"
#include "../../inc/Server.hpp"

Notice::Notice() {}

Notice::~Notice() {}

void Notice::execute(Client* client, std::string arguments) 
{
    // Extraire la cible et le message
    std::istringstream iss(arguments);
    std::string target;
    std::string message;

    iss >> target;
    std::getline(iss, message);

    Server* server = Server::getInstance();
    std::map<int, Client*> clientsList = server->getClients();
    std::map<std::string, Channel*> channelsList = server->getChannels();

    // Vérifier que la cible et le message ne sont pas vides
    if (target.empty() || message.empty()) {
        client->reply(ERR_NEEDMOREPARAMS(client->getNickname(), "NOTICE"));
        return;
    }

    // Construire le message NOTICE
    std::string noticeMessage = ":" + client->getPrefix() + " NOTICE " + target + " :" + message;

    // Envoyer le NOTICE à la cible
    if (target[0] == '#') 
    {
        // Si la cible est un canal, diffuser le message à tous les membres
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
        // Si la cible est un utilisateur, envoyer directement
        bool found = false;
        for (std::map<int, Client*>::iterator it = clientsList.begin(); it != clientsList.end(); ++it)
        {
            if (target == it->second->getNickname())
            {
                //le destinataire est trouve dans les clients presents dans le serveur.
                it->second->write(noticeMessage);
                found = true;
                break ;
            }
        }
        if (found == false)
            client->reply(ERR_NOSUCHNICK(client->getNickname(), target));
    }
}
