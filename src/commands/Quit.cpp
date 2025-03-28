#include "../../inc/commands/Quit.hpp"
#include "../../inc/Server.hpp"
#include "../../inc/Channel.hpp"

Quit::Quit() {}

Quit::~Quit() {}

void Quit::execute(Client* client, std::string arguments)
{
    if (client->getState() == NOT_REGISTERED)
    {
        client->reply(ERR_NOTREGISTERED());
        return;
    }
    
    std::string quitMessage = "Client has quit";
    if (!arguments.empty())
    {
        if (arguments[0] == ':')
            arguments.erase(0, 1);
        quitMessage = arguments;
    }
    Server* server = Server::getInstance();
    std::map<std::string, Channel*> channels = server->getChannels();

    for (std::map<std::string, Channel*>::iterator it = channels.begin(); it != channels.end(); ++it)
    {
        it->second->broadcast(client->getNickname() + " has left the channel: " + quitMessage, client);
        it->second->removeClient(client);
    }

    int clientFd = client->getFd();
    server->getClients().erase(clientFd);
    close(clientFd);
    delete client;

    std::cout << "Client " << clientFd << " has disconnected: " << quitMessage << std::endl;
}