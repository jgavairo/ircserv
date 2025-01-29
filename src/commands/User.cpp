#include "../../inc/commands/ACommand.hpp"

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
    // for (size_t i = 0; i < splittedArgs.size(); i++)
    //     std::cout << "splittedargs[" << i << "] = " << splittedArgs[i] << std::endl;
    if (splittedArgs.size() < 3)
    {
        client->reply(ERR_NEEDMOREPARAMS(client->getNickname(), std::string("USER")));
        return ;
    }
    else
    {
        client->setUsername(splittedArgs[0]);
        client->setRealname(splittedArgs[3]);
        client->setState(REGISTERED);
        std::cout << "----Command 'USER' has been executed on client " << client->getFd() << ". her username is now " << client->getUsername() << "----" << std::endl;
        std::cout << "                                                  " << "her realname is now " << client->getRealname() << "----" << std::endl;
    }
}
    