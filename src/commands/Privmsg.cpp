#include "../../inc/commands/Privmsg.hpp"
#include "../../inc/Channel.hpp"
#include "../../inc/Server.hpp"

Privmsg::Privmsg() {}

Privmsg::~Privmsg() {}

void Privmsg::execute(Client* client, std::string arguments)
{
    std::istringstream iss(arguments);
    std::string channelsNames;

    while (true)
    {
        iss >> channelsNames;
        if (channelsNames[0] == '#')
        {
            std::vector<std::string> channelsTargets;
            std::vector<std::string> clientsTargets;
            while (channelsNames[0] != ':')
            {
                if (channelsNames[0] == '#')
                    channelsTargets.push_back(channelsNames);
                else    
                    clientsTargets.push_back(channelsNames);
                iss >> channelsNames;
            }
            if (channelsNames[0] == ':')
            {
                std::string result, tmp;
                getline(iss, tmp, '\0');
                result = channelsNames + tmp;
                Server* server = Server::getInstance();
                std::map<std::string, Channel*> channelsList = server->getChannels();
                std::map<int, Client*> clientsList = server->getClients();
                for (size_t i = 0 ; i < channelsTargets.size(); ++i)
                {
                    std::string message = ":" + client->getPrefix() + " PRIVMSG " + channelsTargets[i] + " " + result; 
                    channelsList[channelsTargets[i]]->broadcast(message, client);
                }
                for (size_t i = 0 ; i < clientsTargets.size(); ++i)
                {
                    for (std::map<int, Client*>::iterator it = clientsList.begin(); it != clientsList.end(); ++it)
                    {
                        //print les clients target[i] voir si ils sont correct pour etre trouves
                        if (clientsTargets[i] == it->second->getNickname())
                            it->second->write(":" + client->getPrefix() + " PRIVMSG " + clientsTargets[i] + " " + result);
                    }    
                }
                return;
            }
        }
        //error no channel
        return;
    }
}