#include "../../inc/commands/Quit.hpp"
#include "../../inc/Server.hpp"
#include "../../inc/Channel.hpp"

Quit::Quit() {}

Quit::~Quit() {}

void Quit::execute(Client* client, std::string arguments)
{
    std::string quitMessage = "Client has quit";

    if (!arguments.empty())
    {
        if (arguments[0] == ':')
            arguments.erase(0, 1);
        quitMessage = arguments;
    }

    // if (client->getState() != REGISTERED)
    // {
    //     client->reply(ERR_NOTREGISTERED());
    //     return;
    // }

    Server* server = Server::getInstance();
    std::map<std::string, Client*> channels = server->getChannels();

    for (std::map<std::string, Channel*>::iterator it = channels.begin(); it != channels.end(); ++it)
    {
        it->second->broadcast(client->getNickname() + " has left the channel: " + quitMessage, client);
        it->second->removeClient(client);
    }

    int clientFd = client->getFd();
    //shutdown(clientFd, SHUT_RDWR); // Forcer la fermeture de la connexion
    server->getClients().erase(clientFd);
    close(clientFd);
    delete client;

    std::cout << "Client " << clientFd << " has disconnected: " << quitMessage << std::endl;
}