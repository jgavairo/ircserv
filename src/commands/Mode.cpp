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

    if (channelName.empty())//channelName ou mode est vide
    {
        client->reply(ERR_NEEDMOREPARAMS(client->getNickname(), "MODE"));
        return;
    }

    Server* server = Server::getInstance();//recuperer l'instance du serveur
    std::map<std::string, Channel*>& channels = server->getChannels();//recuperer la liste des channels

    if (channels.find(channelName) == channels.end())//channelName n'existe pas
    {
        client->reply(ERR_NOSUCHCHANNEL(client->getNickname(), channelName));
        return;
    }

    Channel* channel = channels[channelName];//recuperer le channel
    // Si aucun mode n'est spécifié, retourner les modes actuels du canal
    if (mode.empty())
    {
        std::string currentModes = channel->getAllModes();
        client->reply(RPL_CHANNELMODEIS(client->getNickname(), channelName, currentModes));
        return;
    }
    if (!channel->isOperator(client))//Arret si le client n'est pas operateur
    {
        client->reply(ERR_CHANOPRIVSNEEDED(client->getNickname(), channelName));
        return;
    }
    // Appliquer les changements de mode au canal
    if (mode == "+i")//channel sur invitation uniquement
        channel->setInviteOnly(true, client, channel, channelName);
    else if (mode == "-i")
        channel->setInviteOnly(false, client, channel, channelName);
    else if (mode == "+t")
        channel->setTopicRestricted(true, client, channel, channelName);
    else if (mode == "-t")
        channel->setTopicRestricted(false, client, channel, channelName);
    else if (mode == "+k")//check les cas a gerer en bas
    {
        if (param.empty()) // Vérifier si le mot de passe est vide
        {
            client->reply(ERR_NEEDMOREPARAMS(client->getNickname(), mode));
            return;
        }
        if (param[0] == '#')
        {
            client->reply(ERR_PASSWDMISMATCH(client->getNickname()));
            return;
        }
        channel->setPassword(param);
        channel->broadcast(NOTICE_PASSWORD_SET(client->getNickname(), channelName), NULL);
    }
    else if (mode == "-k")
    {
        channel->setPassword("");
        channel->broadcast(NOTICE_PASSWORD_UNSET(client->getNickname(), channelName), NULL);
    }
    else if (mode == "+o")
    {
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
        size_t newLimit = std::atoi(param.c_str());
        if (newLimit < channel->getUserCount()) // Vérifier si la nouvelle limite est inférieure au nombre actuel d'utilisateurs
        {
            client->reply(ERR_CANNOTSETLIMIT(client->getNickname(), channelName));
            return;
        }
        channel->setUserLimit(newLimit);
        channel->broadcast(NOTICE_USER_LIMIT_SET(client->getNickname(), channelName, param), NULL);
    }
    else if (mode == "-l")
    {
        channel->setUserLimit(0);
        channel->broadcast(NOTICE_USER_LIMIT_UNSET(client->getNickname(), channelName), NULL);
    }
    else
    {
        client->reply(ERR_UNKNOWNMODE(client->getNickname(), mode));
        return;
    }
    //envoyer le message de changement de mode
    channel->broadcast(":" + client->getNickname() + " MODE " + channelName + " " + mode + " " + param, client);
}

//Pour +k, cas a gerer : 
//si le mot de passe commence par #
//si rien nest entrer apres +k (cela renvient a -k)