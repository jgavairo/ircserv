#include "Channel.hpp"

Channel::Channel(const std::string& name) :_name(name), _topic("") {}

Channel::~Channel() {}

void Channel::addClient(Client* client) 
{

}

void Channel::removeClient(Client* client)
{

}

void Channel::displayClients() const
{
    _
    std::cout << "list clients connected in [" << _name << "] :" << std::endl;
    for (size_t i = 0; i < _clients.size(); i++)
        std::cout << _clients[i]->getNickname() << "\n";
    std::cout << std::endl;
}