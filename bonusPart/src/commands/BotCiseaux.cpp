#include "../../inc/commands/BotCiseaux.hpp"
#include "../../inc/Server.hpp"
#include "../../inc/Channel.hpp"
#include <ctime>

BotCiseaux::BotCiseaux()
{
    _output["Pierre !"] = "pierre";
    _output["Feuille !"] = "feuille";
    _output["Ciseaux !"] = "ciseaux";
}

BotCiseaux::~BotCiseaux() {}

void BotCiseaux::execute(Client* client, std::string arguments)
{
    if (client->getNickname() == "ircbot")
    {
        std::string channel = arguments.erase(0, arguments.find_first_not_of(" \t"));
        srand(time(0));
        std::map<std::string, std::string>::iterator it = _output.begin();
        int randomIndex = rand() % _output.size();
        for (int i = 0; i < randomIndex; ++i)
            ++it;
        if (it->second == "feuille")
        {
            client->_channels[channel]->broadcast(NOTICE_BOT_PFC(client->getPrefix(), channel, it->first, "Tu as gagné !"), client);
            return;
        }
        else if (it->second == "pierre")
        {
            client->_channels[channel]->broadcast(NOTICE_BOT_PFC(client->getPrefix(), channel, it->first, "Tu as perdu !"), client);
            return;
        }
        else
        {
            client->_channels[channel]->broadcast(NOTICE_BOT_PFC(client->getPrefix(), channel, it->first, "Egalité !"), client);
            return;
        }
    }
}