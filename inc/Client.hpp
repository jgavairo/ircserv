#pragma once

#include "lib.hpp"

class Channel;

enum ClientState 
{
    NOT_REGISTERED,
    WAITING_FOR_NICK,
    WAITING_FOR_USER,
    REGISTERED
};

class Client
{
    private:
        int _fd;
        ClientState _state;
        std::string _nickname;
        std::string _username;
        std::string _hostname;
        std::string _realname;
        std::string _password;
    public:
        Client(int fd);
        ~Client();

        std::string getPrefix() const;
        const std::string& getNickname() const;
        const std::string& getUsername() const;
        const std::string& getHostname() const;
        const std::string& getRealname() const;
        int getFd() const;
        ClientState getState() const;

        void setNickname(const std::string& nickname);
        void setUsername(const std::string& nickname);
        void setRealname(const std::string& nickname);
        void setState(ClientState state);

        void reply(const std::string& reply);
        void userReply(const std::string& message);
        void write(const std::string& message);

        void leaveChannel(Channel* channel);

        std::map<std::string, Channel*> _channels; // liste des channels sur lesquels le client est connecte, a passer en PRIVE
};

