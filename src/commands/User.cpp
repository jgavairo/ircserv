#include "../../inc/commands/User.hpp"

User::User() {}

User::~User() {}

void User::execute(Client* client, std::string arguments)
{
    if (client->getState() == REGISTERED)
    {
        client->reply(ERR_ALREADYREGISTERED(client->getNickname()));
        return ;
    }
    std::vector<std::string> splittedArgs(splitArguments(arguments));
    for (size_t i = 0; i < splittedArgs.size(); i++)
        std::cout << "splittedargs[" << i << "] = " << splittedArgs[i] << std::endl;
    if (splittedArgs.size() < 3)
    {
        client->reply(ERR_NEEDMOREPARAMS(client->getNickname(), std::string("USER")));
        return ;
    }
    else
    {
        if (!splittedArgs[3].empty() && splittedArgs[3][0] == ' ') splittedArgs[3].erase(0, 1);
        if (!splittedArgs[3].empty() && splittedArgs[3][0] == ':') splittedArgs[3].erase(0, 1);

        if (splittedArgs[0].empty() || splittedArgs[3].empty()) 
        {
            std::cerr << "Error: Invalid USER command syntax.\n";
            return;
        }

        client->setUsername(splittedArgs[0]);
        client->setRealname(splittedArgs[3]);
        if (client->getState() == WAITING_FOR_USER)
        {
            client->setState(REGISTERED);
            client->reply(RPL_WELCOME(client->getNickname()));
        }
        else if (client->getState() == NOT_REGISTERED)
            client->setState(WAITING_FOR_NICK);
        std::cout << "----Command 'USER' has been executed on client " << client->getFd() << ". her username is now " << client->getUsername() << "----" << std::endl;
        std::cout << "                                                  " << "her realname is now " << client->getRealname() << "----" << std::endl;
    }
}
    