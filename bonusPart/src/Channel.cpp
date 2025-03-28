#include "../inc/Channel.hpp"
#include "../inc/Server.hpp"

Channel::Channel(const std::string& name) :_name(name), _topic(""), _empty(true), _inviteOnly(false), _topicRestricted(false), _userLimit(0) {}

Channel::~Channel()
{
    for (std::map<std::string, Client*>::iterator it = _clients.begin(); it != _clients.end(); ++it)
    {
        if (it->second)
        it->second->_channels.erase(_name);
    }
    
    _clients.clear();
    _operators.clear();
    _invitedClients.clear();
}
    
const std::string Channel::getName() const { return _name; }

const std::string Channel::getTopic() const { return _topic; }

void Channel::setTopic(const std::string& newTopic) { _topic = newTopic; }

void Channel::addClient(Client* client) 
{
    if (_clients.find(client->getNickname()) != _clients.end())
    {
        client->userReply("NOTICE: this client is already present in this channel.");
        return;
    }
    if (client->_channels.find(this->_name) != client->_channels.end())
    {
        std::cerr << "ERROR: this channel is already joined by this client." << std::endl;
        return;
    }
    else
    {
        _empty = false;
        _clients[client->getNickname()] = client;
        client->_channels[this->_name] = this;
    }
}

bool Channel::isEmpty() const { return _empty; }

const std::vector<std::string> Channel::getNamesClients() const
{
    std::vector<std::string> list;
    for (std::map<std::string, Client*>::const_iterator it = _clients.begin(); it != _clients.end(); ++it)
        list.push_back(it->first);
    return list;
}

void Channel::broadcast(const std::string& message, Client* sender)
{
    for (std::map<std::string, Client*>::iterator it = _clients.begin(); it != _clients.end(); ++it)
    {
        if (it->second != sender)
            it->second->userReply(message);
    }
}

void Channel::updateNickname(const std::string& old_, const std::string& new_)
{
    std::map<std::string, Client*>::iterator it = _clients.find(old_);
    if (it != _clients.end())
    {
        Client* clientPtr = it->second;
        _clients.erase(it);
        _clients.insert(std::make_pair(new_, clientPtr));
    }
}
void Channel::removeClient(Client* client)
{
    if (_clients.find(client->getNickname()) != _clients.end())
    {
        _clients.erase(client->getNickname());
        client->leaveChannel(this);
        std::cout << "Client " << client->getNickname() << " removed from channel " << _name << std::endl;

        if (_clients.empty())
        {
            std::cout << "Channel " << _name << " is now empty and will be deleted.\n";
            _empty = true;
            
            Server* server = Server::getInstance();
            server->removeEmptyChannel(_name);
        }
        else if (_clients.size() == 1 && _clients.find("ircbot") != _clients.end())
        {
            removeClient(_clients["ircbot"]);
        }
        else
        {
            if (isOperator(client))
                removeOperator(client->getNickname());
        }
    }
}

void Channel::setInviteOnly(bool inviteOnly, Client* client, Channel* channel)
{
    if (inviteOnly && _allModes.find('i') != std::string::npos)
    {
        client->userReply(NOTICE_MODE_ALREADY_ACTIVE(client->getNickname(), channel->getName(), "i"));
        return;
    }
    _inviteOnly = inviteOnly;

    if (inviteOnly)
    {
        channel->broadcast(NOTICE_INVITE_ONLY_SET(client->getNickname(), channel->getName()), NULL);
        std::cout << "[+i] Channel " << _name << " est que sur invitation." << std::endl;
        _allModes += "i";
    }
    else{
        std::cout << "[-i] Channel " << _name << " n'est plus sur invitation." << std::endl;
        size_t pos = _allModes.find("i");
        if (pos != std::string::npos)
        {
            channel->broadcast(NOTICE_INVITE_ONLY_UNSET(client->getNickname(), channel->getName()), NULL);
            _allModes.erase(pos, 1);
        }
        else
            client->userReply(NOTICE_INVITE_NO_SET(client->getNickname(), channel->getName()));
    }
}

bool Channel::isInviteOnly() const
{
    return _inviteOnly;
}

void Channel::setTopicRestricted(bool topicRestricted, Client* client, Channel* channel)
{
    if (topicRestricted && _allModes.find('t') != std::string::npos)
    {
        client->userReply(NOTICE_MODE_ALREADY_ACTIVE(client->getNickname(), channel->getName(), "t"));
        return;
    }
    _topicRestricted = topicRestricted;
    if (topicRestricted){
        channel->broadcast(NOTICE_TOPIC_RESTRICTED_SET(client->getNickname(), channel->getName()), NULL);
        std::cout << "[+t] Channel " << _name << " is now topic restricted." << std::endl;
        _allModes += "t";
    }
    else{
        std::cout << "[-t] Channel " << _name << " is no longer topic restricted. " << std::endl;
        size_t pos = _allModes.find("t");
        if (pos != std::string::npos)
        {
            channel->broadcast(NOTICE_TOPIC_RESTRICTED_UNSET(client->getNickname(), channel->getName()), NULL);    
            _allModes.erase(_allModes.find("t"), 1);
        }
        else
            client->userReply(NOTICE_TOPIC_NO_SET(client->getNickname(), channel->getName()));
    }
}

bool Channel::isTopicRestricted() const
{
    return _topicRestricted;
}

void Channel::setPassword(const std::string& password, Client* client, Channel* channel)
{
     _password = password;
    if (!password.empty()){
        std::cout << "[+k] Channel " << _name << " est proteger par le mot de passe : |"<< _password <<"|" << std::endl;
        _allModes += "k";
    }
    else{
        std::cout << "[-k] Channel " << _name << " n'est plus proteger par un mot de passe " << std::endl;
        size_t pos = _allModes.find("k");
        if (pos != std::string::npos)
        {
            channel->broadcast(NOTICE_PASSWORD_UNSET(client->getNickname(), channel->getName()), NULL);
            _allModes.erase(_allModes.find("k"), 1);
        }
        else
            client->userReply(NOTICE_PASSWORD_NO_SET(client->getNickname(), channel->getName()));
    }
}

const std::string& Channel::getPassword() const
{
    return _password;
}

void Channel::addOperator(const std::string& nickname)
{
    _operators.insert(nickname);
    std::cout << "[+o] Client " << nickname << " devient un operateur du channel " << _name << std::endl;
}

void Channel::removeOperator(const std::string& nickname)
{
    _operators.erase(nickname);
    std::cout << "[-o] Client " << nickname << " nest plus operateur du channel " << _name << std::endl;
}

bool Channel::isOperator(Client* client) const
{
    return _operators.find(client->getNickname()) != _operators.end();
}

bool Channel::isOperatorByName(std::string client) const
{
    return _operators.find(client) != _operators.end();
}

void Channel::setUserLimit(int limit, Client* client, Channel* channel)
{
    _userLimit = limit;
    if (limit > 0){
        std::cout << "[+l] Channel " << _name << " est limiter a " << limit << " users." << std::endl;
        _allModes += "l";
    }
    else{
        size_t pos = _allModes.find("l");
        if (pos != std::string::npos)
        {
            channel->broadcast(NOTICE_USER_LIMIT_UNSET(client->getNickname(), channel->getName()), NULL);
            _allModes.erase(_allModes.find("l"), 1);
        }
        else
            client->userReply(NOTICE_USER_LIMIT_NO_SET(client->getNickname(), channel->getName()));
        std::cout << "[-l] Channel " << _name << " na plus de limite dutilisateur" << std::endl;

    }
}

size_t Channel::getUserLimit() const
{
    return _userLimit;
}

int Channel::getUserCount() const
{
    return _clients.size();
}

std::string Channel::getAllModes() const
{
    return _allModes;
}

void Channel::setInitialOperator(Client* client)
{
    _operators.insert(client->getNickname());
}

bool Channel::isInvited(Client* client) const
{
    return _invitedClients.find(client->getNickname()) != _invitedClients.end();
}

void Channel::inviteClient(const std::string& nickname)
{
    _invitedClients.insert(nickname);
}

void Channel::removeInvite(const std::string& nickname)
{
    std::set<std::string>::iterator it = _invitedClients.find(nickname);
    if (it != _invitedClients.end())
        _invitedClients.erase(it);
}
