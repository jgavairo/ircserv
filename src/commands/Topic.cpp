#include "../../inc/commands/Topic.hpp"
#include "../../inc/Channel.hpp"
#include "../../inc/Server.hpp"

Topic::Topic() {}

Topic::~Topic() {}

void Topic::execute(Client* client, std::string arguments)
{
    if (arguments.empty())
    {
        client->reply(ERR_NEEDMOREPARAMS(client->getNickname(), "TOPIC"));
        return ;
    }
    std::istringstream iss(arguments);
    std::string channel, newTopic;

    iss >> channel;
    getline(iss, newTopic);

    newTopic.erase(0, newTopic.find_first_not_of(" :"));
    std::cout << "Channel is: [" << channel << "]" << std::endl;
    std::cout << "new Topic is: [" << newTopic << "]" << std::endl;
    (void)client;
}