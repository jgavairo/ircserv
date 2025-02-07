#include "../../inc/commands/Privmsg.hpp"
#include "../../inc/Channel.hpp"
#include "../../inc/Server.hpp"

Privmsg::Privmsg() {}

Privmsg::~Privmsg() {}

void Privmsg::execute(Client* client, std::string arguments)
{
    (void) client;
    std::vector<std::string> splittedArg = splitArguments(arguments);
    
    std::string
}