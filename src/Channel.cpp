#include "../inc/Channel.hpp"

Channel::Channel(const std::string& name) :_name(name), _topic("") {}

Channel::~Channel() {}

const std::string Channel::getName() const { return _name; }

void Channel::addClient(Client* client) 
{
    if (_clients.find(client->getNickname()) != _clients.end())
    {
        std::cerr << "ERROR: this client is already present in this channel." << std::endl;
        return;
    }
    if (client->_channels.find(this->_name) != client->_channels.end())
    {
        std::cerr << "ERROR: this channel is already joined by this client." << std::endl;
        return;
    }
    _clients[client->getNickname()] = client;
    client->_channels[this->_name] = this;
}

void Channel::broadcast(const std::string& message, Client* sender)
{
    for (std::map<std::string, Client*>::iterator it = _clients.begin(); it != _clients.end(); ++it)
    {
        if (it->second != sender)
            it->second->reply(message);
    }
}

void Channel::updateNickname(const std::string& old_, const std::string& new_)
{
    std::map<std::string, Client*>::iterator it = _clients.find(old_);
    if (it != _clients.end())
    {
        Client* clientPtr = it->second;
        _clients.erase(it);
        _clients.insert(std::make_pair(new_, clientPtr));
    }
}
void Channel::removeClient(Client* client)
{
    if (_clients.find(client->getNickname()) != _clients.end())
    {
        _clients.erase(client->getNickname());
        client->leaveChannel(this);
        std::cout << "Client " << client->getNickname() << " removed from channel " << _name << std::endl;

        //si le channel est vide, on le supprime
        if (_clients.empty())
        {
            std::cout << "Channel " << _name << " is now empty and will be deleted.\n";

            //Ajouter la fonction d'instance du server 

            // Server* server = Server::getInstance(); // Si tu as un singleton Server
            //server->removeEmptyChannel(_name);
        }
    }
}

void Channel::displayClients() const
{
    for (std::map<std::string, Client*>::const_iterator it = _clients.begin(); it != _clients.end(); ++it)
        std::cout << it->first << " : " << it->second->getNickname() << std::endl;
}