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
        std::string _buffer;
        bool        _authenticated;
    public:
        Client(int fd);
        ~Client();

        std::string getPrefix() const;
        const std::string& getNickname() const;
        const std::string& getUsername() const;
        const std::string& getHostname() const;
        const std::string& getRealname() const;
        const std::string& getPassword() const;
        int getFd() const;
        ClientState getState() const;
        std::string& getBuffer();
        
        void setNickname(const std::string& nickname);
        void setUsername(const std::string& nickname);
        void setPassword(const std::string& password);
        void setRealname(const std::string& nickname);
        void setState(ClientState state);
        void appendToBuffer(const std::string& data);
        
        void setAuth(bool state);
        
        void reply(const std::string& reply);
        void userReply(const std::string& message);
        void write(const std::string& message);
        
        bool isAuthenticated();
        bool isOperator();
        
        void clearBuffer();
        void leaveChannel(Channel* channel);
        
        Channel* searchChannel(const std::string& name);
        std::map<std::string, Channel*> _channels;
};

