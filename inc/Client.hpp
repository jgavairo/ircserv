#pragma once

#include "lib.hpp"

class Channel;

class Client
{
    private:
        int _fd;
        std::string _nickname;
        std::string _username;
        std::string _hostname;
        std::string _realname;
        std::set<Channel*> _channels; // liste des channels sur lesquels le client est connecte
        bool _authenticated; //le client est il auth
    public:
        Client(int fd);
        ~Client();

        const std::string& getNickname() const;
        const std::string& getUsername() const;
        const std::string& getHostname() const;
        const std::string& getRealname() const;
        int getFd() const;

        void setNickname(const std::string& nickname);
        void setUsername(const std::string& nickname);
        void setRealname(const std::string& nickname);
        
        void authentificate();
        bool isAuthentificated() const;
};

