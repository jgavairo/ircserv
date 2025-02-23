#include "../../inc/commands/AddBot.hpp"
#include "../../inc/Server.hpp"
#include "../../inc/Channel.hpp"

AddBot::AddBot() {}

AddBot::~AddBot() {}

void AddBot::execute(Client* client, std::string arguments)
{
    if (client->getState() != REGISTERED)
    {
        client->reply(ERR_NOTREGISTERED());
        return;
    }
    
    std::istringstream iss(arguments);
    std::string channel;
    iss >> channel;
    
    if (channel.empty())
    {
        client->userReply(NOTICE_BOT_NEED_MORE_PARAM(client->getNickname(), channel));
        client->reply(ERR_NEEDMOREPARAMS(client->getNickname(), "ADDBOT"));
        return ;
    }

    
    Server* server = Server::getInstance();
    std::map<std::string, Channel*> serverChannel = server->getChannels();
    
    Client* bot = NULL;
    std::map<int, Client*>& clients = Server::getInstance()->getClients();
    for (std::map<int, Client*>::iterator it = clients.begin(); it != clients.end(); ++it)
    {
        if (it->second->getNickname() == "ircbot")
        {
            bot = it->second;
            break;
        }
    }
    
    if (!serverChannel[channel]->isOperator(client))
    {
        client->reply(ERR_CHANOPRIVSNEEDED(client->getNickname(), channel));
        return ;
    }

    if (!bot)
    {
        client->reply(ERR_NOSUCHNICK(client->getNickname(), "ircbot"));
        return ;
    }

    if (serverChannel.find(channel) != serverChannel.end())
    {
        if (serverChannel[channel]->isInvited(bot)) // A FAIRE -> supprimmer un invite si il est kick (pouvoir reinviter le bot en cas de kick)
        {
            client->userReply(NOTICE_BOT_ALREADY_PRESENT(client->getNickname(), channel));
            return ;
        }
        serverChannel[channel]->inviteClient(bot->getNickname());
        serverChannel[channel]->addClient(bot);
        
        std::string joinMessage = RPL_JOIN(bot->getPrefix(), channel);
        serverChannel[channel]->broadcast(joinMessage, NULL);
    }
    else
    {
        client->reply(ERR_NOSUCHCHANNEL(client->getNickname(), channel));
        return ;
    }
}