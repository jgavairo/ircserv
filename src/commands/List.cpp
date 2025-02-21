#include "../../inc/commands/List.hpp"
#include "../../inc/Server.hpp"
#include "../../inc/Channel.hpp"

List::List() {}

List::~List() {}

void List::execute(Client* client, std::string arguments)
{
    if (client->getState() != REGISTERED)
    {
        client->reply(ERR_NOTREGISTERED());
        return ;
    }
    (void) arguments;
    
    Server* server = Server::getInstance();
    std::map<std::string, Channel*> serverChannels = server->getChannels();

    client->reply(RPL_LISTSTART(client->getNickname()));
    for (std::map<std::string, Channel*>::const_iterator it = serverChannels.begin(); it != serverChannels.end(); ++it)
    {
        const Channel* channel = it->second;
        std::ostringstream userCount;
        userCount << channel->getUserCount() << std::ends;
        std::string strCount = userCount.str();

        client->reply(RPL_LIST(client->getNickname(), channel->getName(), strCount, channel->getTopic()));
    }
    client->reply(RPL_LISTEND(client->getNickname()));
}