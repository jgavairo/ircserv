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
    // for (size_t i = 0; i < splittedArgs.size(); i++)
    //     std::cout << "splittedargs[" << i << "] = " << splittedArgs[i] << std::endl;
    if (splittedArgs.size() < 4)
    {
        client->reply(ERR_NEEDMOREPARAMS(client->getNickname(), std::string("USER")));
        return ;
    }
    else
    {
        //Verification de la presence de tous les arguments.
        size_t pos = arguments.find(':');
        if (splittedArgs[0].empty() || pos == std::string::npos) 
        {
            std::cerr << "Error: Invalid USER command syntax.\n";
            return;
        }
        //Extraction complete du realName.
        std::string realName = arguments.substr(pos + 1, arguments.find(CRLF));

        //Assignations des bonnes valeurs
        client->setUsername(splittedArgs[0]);
        client->setRealname(realName);

        //Gestion des etats de login
        if (client->getState() == NOT_REGISTERED)
        {
            client->setState(WAITING_FOR_NICK); 
            client->reply("NOTICE * :Please specify your nickname using the NICK command");
        }
        else if (client->getState() == WAITING_FOR_USER)
            client->setState(REGISTERED);
        if (client->getState() == REGISTERED)
            client->reply(RPL_WELCOME(client->getNickname()));
    }
}