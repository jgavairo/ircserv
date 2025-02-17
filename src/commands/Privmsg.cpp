#include "../../inc/commands/Privmsg.hpp"
#include "../../inc/Channel.hpp"
#include "../../inc/Server.hpp"

Privmsg::Privmsg() {}

Privmsg::~Privmsg() {}

void Privmsg::execute(Client* client, std::string arguments)
{
    std::istringstream iss(arguments);
    std::string newArguments;
    std::vector<std::string> clientsTargets;
    std::vector<std::string> channelsTargets;

    while (true)
    {
        //Pas d'arguments, provoque une erreur.
        if (arguments.empty())
        {
            client->reply(ERR_NEEDMOREPARAMS(client->getNickname(), "PRIVMSG"));
            return ;
        }

        //Recuperation des destinataires (Channel si #, sinon client) jusqu'a trouver ':' qui signale le debut du message.
        iss >> newArguments;
        while (newArguments[0] != ':')
        {
            if (newArguments[0] == '#')
                channelsTargets.push_back(newArguments);
            else    
                clientsTargets.push_back(newArguments);
            iss >> newArguments;
        }

        //Verification que des destinataires sont bien presents, sinon une erreur se produit.
        if (channelsTargets.empty() && clientsTargets.empty())
        {
            client->reply(ERR_NORECIPIENT(client->getNickname()));
            return ;
        }

        //Si ':' est bien trouve, alors ce qui suit sera le message a delivrer.
        if (newArguments[0] == ':')
        {
            std::string result, tmp, noText;
            getline(iss, tmp, '\0');
            result = newArguments + tmp;
            
            //Si aucun message n'est present (donc rien a envoyer, une erreur se produit).
            if ((result.size() == 1 && result[0] == ':') || result.empty())
                {
                    client->reply(ERR_NOTEXTTOSEND(client->getNickname()));
                    return ;
                }

                //Recuperation des infos serveur.
                Server* server = Server::getInstance();
                std::map<std::string, Channel*> channelsList = server->getChannels();
                std::map<int, Client*> clientsList = server->getClients();
                
                //Des channels
                if (!channelsTargets.empty())
                {
                    for (size_t i = 0 ; i < channelsTargets.size(); ++i)
                    {
                        //Mise au bon format du message.
                        std::string message = ":" + client->getPrefix() + " PRIVMSG " + channelsTargets[i] + " " + result;

                        //Si le channel existe.
                        if (channelsList.find(channelsTargets[i]) != channelsList.end())
                        {
                            if (channelsList[channelsTargets[i]]->_clients.find(client->getNickname()) != channelsList[channelsTargets[i]]->_clients.end())
                                channelsList[channelsTargets[i]]->broadcast(message, client);
                            else
                            {
                                client->reply(ERR_NOTONCHANNEL(client->getNickname(), channelsTargets[i]));
                            }
                        }
                        else
                        {
                            //Si le channel n'existe pas.
                            client->reply(ERR_CANNOTSENDTOCHAN(client->getNickname(), channelsTargets[i]));
                        }
                    }
                }
                
                //Des clients
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
                                //le destinataire est trouve dans les clients presents dans le serveur.
                                it->second->write(":" + client->getPrefix() + " PRIVMSG " + clientsTargets[i] + " " + result);
                                found = true;
                                break ;
                            }
                        }
                        //si le client n'as pas ete trouver, alors on retourne une erreur.
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