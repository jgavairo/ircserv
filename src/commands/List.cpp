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

}