#pragma once

#include "lib.hpp"

class Client;

class Channel
{
    private:
        std::string _name;
        std::string _password;
        std::string _topic;
        std::set<Client*> _whitelist;
    public:
        std::vector<Client*> _clients;
        Channel(const std::string& name);
        ~Channel();
        void addClient(Client* client);
        void removeClient(Client* client);
        void displayClients() const;
};