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
    std::string _mode;
    std::string _allModes;
    std::set<std::string> _operators;
    std::set<std::string> _invitedClients;
    bool    _empty;
    bool    _inviteOnly;
    bool    _topicRestricted;
    size_t  _userLimit;
public:
    std::map<std::string, Client*> _clients;
    Channel(const std::string& name);
    ~Channel();

    const std::vector<std::string> getNamesClients() const;
    const std::string getName() const;
    const std::string getTopic() const;
    const std::string& getPassword() const;
    std::string getAllModes() const;
    size_t  getUserCount() const;
    size_t  getUserLimit() const;

    void setTopic(const std::string& newTopic);
    void setTopicRestricted(bool topicRestricted, Client* client, Channel* channel);
    void setInviteOnly(bool inviteOnly, Client* client, Channel* channel);
    void setPassword(const std::string& password, Client* client, Channel* channel);
    void setUserLimit(int limit, Client* client, Channel* channel);
    void setInitialOperator(Client* client);
    
    void removeInvite(const std::string& nickname);
    void updateNickname(const std::string& old_, const std::string& new_);
    void addClient(Client* client);
    void removeClient(Client* client);
    void displayClients() const;
    void broadcast(const std::string& message, Client* sender);
    void addOperator(const std::string& nickname);
    void removeOperator(const std::string& nickname);
    void inviteClient(const std::string& nickname);

    bool isEmpty() const;
    bool isInviteOnly() const;
    bool isTopicRestricted() const;
    bool isOperator(Client* client) const;
    bool isOperatorByName(std::string client) const;
    bool isInvited(Client* client) const;
};