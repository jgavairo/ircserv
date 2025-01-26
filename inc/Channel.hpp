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
        std::map<Client*, bool> _clients;
    public:
        Channel();
        ~Channel();
};