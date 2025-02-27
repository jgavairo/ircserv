#include "../../inc/commands/User.hpp"

User::User() {}

User::~User() {}

void User::execute(Client* client, std::string arguments)
{
    if (!client->isAuthenticated())
    {
        client->reply(ERR_NOTREGISTERED());
        return ;
    }
    if (client->getState() == REGISTERED)
    {
        client->reply(ERR_ALREADYREGISTERED(client->getNickname()));
        return ;
    }
    std::vector<std::string> splittedArgs(splitArguments(arguments));
    if (splittedArgs.size() < 4)
    {
        client->reply(ERR_NEEDMOREPARAMS(client->getNickname(), std::string("USER")));
        return ;
    }
    else
    {
        size_t pos = arguments.find(':');
        if (splittedArgs[0].empty() || pos == std::string::npos) 
        {
            std::cerr << "Error: Invalid USER command syntax.\n";
            return;
        }
        std::string realName = arguments.substr(pos + 1, arguments.find(CRLF));

        client->setUsername(splittedArgs[0]);
        client->setRealname(realName);

        if (client->getState() == NOT_REGISTERED)
        {
            client->setState(WAITING_FOR_NICK); 
            client->userReply(NOTICE_MISS_NICKNAME());
        }
        else if (client->getState() == WAITING_FOR_USER)
            client->setState(REGISTERED);
        if (client->getState() == REGISTERED)
            client->reply(RPL_WELCOME(client->getNickname()));
    }
}