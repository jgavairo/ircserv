#pragma once

#include "lib.hpp"
#include "Client.hpp"

class Client;

class Channel
{
    private:
        std::string _name;
        std::string _password;
        std::string _topic;
        std::set<Client*> _whitelist;
    public:
        std::map<std::string, Client*> _clients;
        Channel(const std::string& name);
        ~Channel();

        const std::string getName() const;

        void addClient(Client* client);
        void removeClient(Client* client);
        void displayClients() const;
        void broadcast(const std::string& message, Client* sender);
};