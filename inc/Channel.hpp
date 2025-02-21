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
    std::set<Client*> _whitelist;//Liste des clients autorisés
    std::string _mode;//Mode du channel
    std::set<std::string> _operators;//Liste des opérateurs
    std::set<std::string> _invitedClients; // Ajout de la liste des clients invités
    bool    _inviteOnly;//Mode +i
    bool    _topicRestricted;//Mode +t
    size_t  _userLimit;//Limite d'utilisateur
    bool    _empty;
public:
    std::map<std::string, Client*> _clients;
    Channel(const std::string& name);
    ~Channel();

    const std::string getName() const;
    const std::string getTopic() const;
    const std::string& getPassword() const;

    void setTopic(const std::string& newTopic);
    bool isEmpty() const;
    
    void updateNickname(const std::string& old_, const std::string& new_);
    void addClient(Client* client);
    const std::vector<std::string> getNamesClients() const;
    void removeClient(Client* client);
    void displayClients() const;
    void broadcast(const std::string& message, Client* sender);


    // Méthodes pour gérer les modes
    void setInviteOnly(bool inviteOnly);
    bool isInviteOnly() const;

    void setTopicRestricted(bool topicRestricted);
    bool isTopicRestricted() const;

    void setPassword(const std::string& password);

    void addOperator(const std::string& nickname);
    void removeOperator(const std::string& nickname);
    bool isOperator(Client* client) const;

    void    setUserLimit(int limit);
    size_t  getUserLimit() const;

    // Nouvelle méthode pour définir un opérateur lors de la création du canal
    void setInitialOperator(Client* client);

    // Nouvelle méthode pour vérifier si un client est invité
    bool isInvited(Client* client) const;
    void inviteClient(const std::string& nickname);
};