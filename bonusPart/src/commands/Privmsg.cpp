#include "../../inc/commands/Privmsg.hpp"
#include "../../inc/Channel.hpp"
#include "../../inc/Server.hpp"

Privmsg::Privmsg() {}

Privmsg::~Privmsg() {}

void Privmsg::execute(Client* client, std::string arguments)
{
    if (client->getState() != REGISTERED)
    {
        client->reply(ERR_NOTREGISTERED());
        return ;
    }

    std::istringstream iss(arguments);
    std::string newArguments;
    std::vector<std::string> clientsTargets;
    std::vector<std::string> channelsTargets;

    while (true)
    {
        if (arguments.empty())
        {
            client->reply(ERR_NEEDMOREPARAMS(client->getNickname(), "PRIVMSG"));
            return ;
        }

        iss >> newArguments;
        while (newArguments[0] != ':')
        {
            if (newArguments[0] == '#')
                channelsTargets.push_back(newArguments);
            else    
                clientsTargets.push_back(newArguments);
            iss >> newArguments;
        }

        if (channelsTargets.empty() && clientsTargets.empty())
        {
            client->reply(ERR_NORECIPIENT(client->getNickname()));
            return ;
        }

        if (newArguments[0] == ':')
        {
            std::string result, tmp, noText;
            std::getline(iss, tmp, '\0');
            result = newArguments + tmp;
            
            if ((result.size() == 1 && result[0] == ':') || result.empty())
                {
                    client->reply(ERR_NOTEXTTOSEND(client->getNickname()));
                    return ;
                }

                Server* server = Server::getInstance();
                std::map<std::string, Channel*> channelsList = server->getChannels();
                std::map<int, Client*> clientsList = server->getClients();
                
                if (!channelsTargets.empty())
                {
                    for (size_t i = 0 ; i < channelsTargets.size(); ++i)
                    {
                        std::string message = ":" + client->getPrefix() + " PRIVMSG " + channelsTargets[i] + " " + result;

                        if (channelsList.find(channelsTargets[i]) != channelsList.end())
                        {
                            if (channelsList[channelsTargets[i]]->_clients.find(client->getNickname()) != channelsList[channelsTargets[i]]->_clients.end())
                                channelsList[channelsTargets[i]]->broadcast(message, client);
                            else
                                client->reply(ERR_NOTONCHANNEL(client->getNickname(), channelsTargets[i]));
                        }
                        else
                            client->reply(ERR_CANNOTSENDTOCHAN(client->getNickname(), channelsTargets[i]));
                    }
                }
                if (!clientsTargets.empty())
                {
                    bool found;
                    for (size_t i = 0 ; i < clientsTargets.size(); ++i)
                    {
                        found = false;
                        for (std::map<int, Client*>::iterator it = clientsList.begin(); it != clientsList.end(); ++it)
                        {
                            if (clientsTargets[i] == it->second->getNickname())
                            {
                                it->second->write(":" + client->getPrefix() + " PRIVMSG " + clientsTargets[i] + " " + result);
                                found = true;
                                break ;
                            }
                        }
                        if (!found)
                            client->reply(ERR_NOSUCHNICK(client->getNickname(), clientsTargets[i]));
                    }
                    return;
                }
                break ; 
        }
        else 
        {
            client->reply(ERR_NOTEXTTOSEND(client->getNickname()));
            return ;
        }
    }
    return;
}