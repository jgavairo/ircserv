#include "../../inc/commands/Join.hpp"
#include "../../inc/Channel.hpp"
#include "../../inc/Server.hpp"

Join::Join() {}

Join::~Join() {}

void Join::execute(Client* client, std::string arguments)
{
    if (client->getState() != REGISTERED)
    {
        client->reply(ERR_NOTREGISTERED());
        return ;
    }
    std::istringstream iss(arguments);
    std::string channelName, keysList;
    iss >> channelName;
    iss >> keysList;
    std::istringstream iss2(channelName), iss3(keysList);
    size_t indexKey = 0;
    Server* server = Server::getInstance();
    std::map<std::string, Channel*>& channels = server->getChannels();
    std::vector<std::string> keys;

    while (std::getline(iss3, keysList, ','))
    {
        keysList.erase(0, keysList.find_first_not_of(" \t"));
        keysList.erase(keysList.find_last_not_of(" \t") + 1);
        keys.push_back(keysList);
    }

    while (std::getline(iss2, channelName, ','))
    {
        channelName.erase(0, channelName.find_first_not_of(" \t"));
        channelName.erase(channelName.find_last_not_of(" \t") + 1);

        if (channelName.empty())
        {
            client->reply(ERR_NEEDMOREPARAMS(client->getNickname(), std::string("JOIN")));
            return;
        }

        if (channelName[0] != '#')
        {
            client->reply(ERR_NOSUCHCHANNEL(client->getNickname(), channelName));
            continue;
        }

        if (channels.find(channelName) == channels.end())
        {
            channels[channelName] = new Channel(channelName);
            channels[channelName]->setInitialOperator(client); // Définir le créateur du canal comme opérateur
        }

        if (channels[channelName]->getUserLimit() > 0 && (channels[channelName]->_clients.size()) >= channels[channelName]->getUserLimit())
        {
            client->reply(ERR_CHANNELISFULL(client->getNickname(), channelName));
            return;
        }

        if (channels[channelName]->isInviteOnly() && !channels[channelName]->isInvited(client))
        {
            client->reply(ERR_INVITEONLYCHAN(client->getNickname(), channelName));
            return;
        }

        if (channels[channelName]->isInvited(client) || channels[channelName]->getPassword().empty())
            channels[channelName]->addClient(client);
        else if (!channels[channelName]->getPassword().empty())
        {
            if (keys.empty() || (indexKey < keys.size() && keys[indexKey] != channels[channelName]->getPassword()))
            {
                client->reply(ERR_BADCHANNELKEY(client->getNickname(), channelName));
                indexKey++;
                continue;
            }
            else
                channels[channelName]->addClient(client);
            indexKey++;
        }

        std::string namesList;
        const std::vector<std::string>& clients = channels[channelName]->getNamesClients();
        for (size_t i = 0; i < clients.size(); ++i) 
        {
            if (!namesList.empty()) 
            {
                namesList += " ";
            }
            if (channels[channelName]->isOperatorByName(clients[i]))
                namesList += "@" + clients[i];
            else
                namesList += clients[i];
        }

        client->write(RPL_JOIN(client->getPrefix(), channelName));
        channels[channelName]->broadcast(RPL_JOIN(client->getPrefix(), channelName), client);

        client->reply(RPL_NAMREPLY(client->getNickname(), channelName, namesList));
        client->reply(RPL_ENDOFNAMES(client->getNickname(), channelName));
        if (!channels[channelName]->getTopic().empty())
            client->reply(RPL_TOPIC(client->getNickname(), channelName, channels[channelName]->getTopic()));
        else
            client->reply(RPL_NOTOPIC(client->getNickname(), channelName));
        std::cout << "Client " << client->getFd() << " joined channel: " << channelName << std::endl;
    }
}