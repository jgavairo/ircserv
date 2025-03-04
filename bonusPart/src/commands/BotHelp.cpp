#include "../../inc/commands/BotHelp.hpp"
#include "../../inc/Server.hpp"
#include "../../inc/Channel.hpp"

BotHelp::BotHelp() {}

BotHelp::~BotHelp() {}

void BotHelp::execute(Client* client, std::string arguments)
{
    if (client->getNickname() == "ircbot")
    {
        arguments.erase(0, arguments.find_first_not_of(" \t"));
        if (client->_channels.find(arguments) != client->_channels.end())
        {
            std::string channel = arguments;
            client->_channels[arguments]->broadcast(NOTICE_BOT_HELP_HEADER(client->getPrefix(), channel), client);
            client->_channels[arguments]->broadcast(NOTICE_BOT_HELP_TIME(client->getPrefix(), channel), client);
            client->_channels[arguments]->broadcast(NOTICE_BOT_HELP_PFC(client->getPrefix(), channel), client);
        }
    }
}