#pragma once

#include "lib.hpp"

class Client;

class Channel
{
    private:
        std::string _name;
        std::string _password;
        std::string _topic;
        std::set<Client*> _whitelist; //liste des clients invites
        std::map<Client*, bool> _clients; //liste des clients et si ils sont auth
    public:
        Channel();
        ~Channel();
};