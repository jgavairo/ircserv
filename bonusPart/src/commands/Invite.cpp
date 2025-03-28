#include "../../inc/commands/Invite.hpp"
#include "../../inc/Channel.hpp"
#include "../../inc/Server.hpp"

Invite::Invite() {}

Invite::~Invite() {}

void Invite::execute(Client* client, std::string arguments)
{
    std::istringstream iss(arguments);
    std::string channel, nickname;
    iss >> nickname;
    iss >> channel;

    if (client->getState() == NOT_REGISTERED)
    {
        client->reply(ERR_NOTREGISTERED());
        return;
    }
    if (nickname.empty() || channel.empty())
    {
        client->reply(ERR_NEEDMOREPARAMS(client->getNickname(), "INVITE"));
        return ;
    }
    Server* server = Server::getInstance();
    const std::map<int, Client*> clientsList = server->getClients();
    for (std::map<int, Client*>::const_iterator it = clientsList.begin(); it != clientsList.end(); ++it)
    {
        if (it->second->getNickname() == nickname)
        {
            if (client->searchChannel(channel))
            {
                if (!client->searchChannel(channel)->isOperator(client))
                {
                    client->reply(ERR_CHANOPRIVSNEEDED(client->getNickname(), channel));
                    return ;
                }
                client->searchChannel(channel)->inviteClient(nickname);
                it->second->reply(RPL_INVITING(nickname, channel, client->getNickname()));
                return ;
            }
            else
            {
                client->reply(ERR_NOTONCHANNEL(client->getNickname(), channel));
                return ;
            }
        }
    }
    client->reply(ERR_NOSUCHNICK(client->getNickname(), nickname));
    return ;
}