#include "../../inc/commands/Nick.hpp"
#include "../../inc/Server.hpp"
#include "../../inc/Channel.hpp"

Nick::Nick() {}

Nick::~Nick() {}

void Nick::execute(Client* client, std::string arguments)
{
    if (!client->isAuthenticated())
    {
        client->reply(ERR_NOTREGISTERED());
        return ;
    }
    if (arguments.empty())
    {
        client->reply(ERR_NONICKNAMEGIVEN(client->getNickname()));
        return ;
    }
    std::istringstream iss(arguments);
    std::string new_nickname;
    iss >> new_nickname;

    Server* server = Server::getInstance();
    std::map<int, Client*>& clientsList = server->getClients();
    for (std::map<int, Client*>::iterator it = clientsList.begin(); it != clientsList.end(); ++it)
    {
        if (it->second->getNickname() == new_nickname)
        {
            std::cerr << "Nickname " << new_nickname << "has already used by client" << it->first << std::endl;
            client->reply(ERR_NICKNAMEINUSE(new_nickname));
            return ;
        }
    }

    std::string old_nick = client->getNickname();
    if (client->getState() == WAITING_FOR_NICK)
    {
        client->setState(REGISTERED);
    }
    else if (client->getState() == NOT_REGISTERED)
        client->setState(WAITING_FOR_USER);
    if (client->getState() == REGISTERED)
        client->reply(RPL_WELCOME(new_nickname));

    for (std::map<std::string, Channel*>::iterator it = client->_channels.begin(); it != client->_channels.end(); ++it)
    {
        it->second->broadcast(RPL_NICK(client->getPrefix(), new_nickname), client);
        std::string noticeMessage = ":" + client->getPrefix() + " NOTICE " + it->second->getName() + " :" + old_nick + " is now known as " + new_nickname;
        it->second->broadcast(noticeMessage, client);
    }

    client->write(RPL_NICK(client->getPrefix(), new_nickname));
    server->updateNickInChannels(old_nick, new_nickname);
    client->setNickname(new_nickname);
    


    std::cout << "\n\n\t---CHECK DES NICK DANS CHAQUE CONTAINER---" << std::endl;
    std::cout << "\n\t---DANS _CLIENTS(SERVER)---" << std::endl;
    std::map<int, Client*>& clientsList2 = server->getClients();
    for (std::map<int, Client*>::iterator itt = clientsList2.begin(); itt != clientsList2.end(); ++itt)
        std::cout << "\t" << itt->second->getNickname() << std::endl;

    std::cout << "\n\t---DANS LES CHANNELS(SERVER)---" << std::endl;
    const std::map<std::string, Channel*>& channelList = server->getChannels();
    for (std::map<std::string, Channel*>::const_iterator it = channelList.begin(); it != channelList.end(); ++it)
    {
        std::cout << "\tfor channel " << it->first << ":" << std::endl;
        std::cout << "\t";
        it->second->displayClients();
        std::cout << std::endl;
    }

    std::cout << "----Command 'NICK' has been executed on client " << client->getFd() << ". her nickname is now " << client->getNickname() << "----" << std::endl;
}