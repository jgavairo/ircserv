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
        std::string _mode; // Ajout de l'attribut _mode

    public:
        std::map<std::string, Client*> _clients;
        Channel(const std::string& name);
        ~Channel();

        const std::string getName() const;

        void updateNickname(const std::string& old_, const std::string& new_);
        void addClient(Client* client);
        void removeClient(Client* client);
        void displayClients() const;
        void broadcast(const std::string& message, Client* sender);

        //void setMode(const std::string& mode); // Ajout de la méthode setMode
        //bool isOperator(Client* client); // Ajout de la méthode isOperator
};