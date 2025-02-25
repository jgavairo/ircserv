#include "../../inc/commands/BotTime.hpp"
#include "../../inc/Server.hpp"
#include "../../inc/Channel.hpp"
#include <ctime>
#include <iomanip>

BotTime::BotTime() {}

BotTime::~BotTime() {}

void BotTime::execute(Client* client, std::string arguments)
{
    if (client->getNickname() == "ircbot")
    {
        arguments.erase(0, arguments.find_first_not_of(" \t"));
        
        std::time_t now = std::time(NULL);
        std::tm* localTime = std::localtime(&now);

        std::ostringstream timeStream;
        timeStream  << std::setw(2) << std::setfill('0') << localTime->tm_hour << ":"
                    << std::setw(2) << std::setfill('0') << localTime->tm_min;

        std::string currentTime = timeStream.str(), channel = arguments;
        client->_channels[channel]->broadcast(NOTICE_BOT_TIME(client->getPrefix(), channel, currentTime), client);
    }
}