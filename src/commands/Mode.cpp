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

    if (channelName.empty() || mode.empty())//channelName ou mode est vide
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
    if (!channel->isOperator(client))//Arret si le client n'est pas operateur
    {
        client->reply(ERR_CHANOPRIVSNEEDED(client->getNickname(), channelName));
        return;
    }

    // Appliquer les changements de mode au canal
    if (mode == "+i")//channel sur invitation uniquement
        channel->setInviteOnly(true);
    else if (mode == "-i")
        channel->setInviteOnly(false);
    else if (mode == "+t")
        channel->setTopicRestricted(true);
    else if (mode == "-t")
        channel->setTopicRestricted(false);
    else if (mode == "+k")//check les cas a gerer en bas
        channel->setPassword(param);
    else if (mode == "-k")
        channel->setPassword("");
    else if (mode == "+o")
        channel->addOperator(param);
    else if (mode == "-o")
        channel->removeOperator(param);
    else if (mode == "+l")
        channel->setUserLimit(std::atoi(param.c_str()));
    else if (mode == "-l")
        channel->setUserLimit(0);
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