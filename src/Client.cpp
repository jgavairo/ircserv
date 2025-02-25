#include "../inc/Client.hpp"
#include "../inc/Channel.hpp"

Client::Client(int fd) 
    : _fd(fd), _state(NOT_REGISTERED), _hostname("localhost") {}

Client::~Client()
{
    // Fermer le file descriptor si ce n'est pas déjà fait
    if (_fd >= 0)
        close(_fd);
    
    // Quitter tous les channels auxquels le client est connecté
    for (std::map<std::string, Channel*>::iterator it = _channels.begin(); it != _channels.end(); ++it)
    {
        if (it->second)
            it->second->removeClient(this);
    }
    _channels.clear();
}

//getters
int Client::getFd() const { return _fd; }
const std::string& Client::getHostname() const { return _hostname; }
const std::string& Client::getRealname() const { return _realname; }
const std::string& Client::getPassword() const { return _password; }
const std::string& Client::getUsername() const { return _username; }
const std::string& Client::getNickname() const { return _nickname; }
ClientState Client::getState() const { return _state; }

//setters
void Client::setNickname(const std::string& nickname) { _nickname = nickname; }
void Client::setUsername(const std::string& username) { _username = username; }
void Client::setPassword(const std::string& password) { _password = password; }
void Client::setRealname(const std::string& realname) { _realname = realname; }
void Client::setState(ClientState state) { _state = state; }

bool Client::isAuthenticated() { return _authenticated; }
bool Client::isOperator() { return _operator; }

void Client::reply(const std::string& reply)
{
    std::string serverName(SERVER_NAME);
    this->write(":" + serverName + " " + reply);
}

void Client::setAuth(bool state) { _authenticated = state; }

void Client::leaveChannel(Channel* channel)
{
    if (_channels.find(channel->getName()) != _channels.end())
    {
        _channels.erase(channel->getName());
        std::cout << "Client " << _nickname << " has left channel " << channel->getName() << std::endl;
    }
}

Channel* Client::searchChannel(const std::string& name)
{
    for (std::map<std::string, Channel*>::iterator it = _channels.begin(); it != _channels.end(); ++it)
    {
        if (it->first == name)
            return it->second;
    }
    return NULL;
}

std::string Client::getPrefix() const 
{
    std::string username = _username.empty() ? "" : "!" + _username;
    std::string hostname = _hostname.empty() ? "" : "@" + _hostname;
	return (_nickname + username + hostname);
}

void Client::write(const std::string& message)
{
    std::string finalMessage = message + CRLF;
    // std::cout << _nickname << message << std::endl;
    if (send(_fd, finalMessage.c_str(), finalMessage.length(), 0) < 0)
        throw (std::runtime_error("writing message from client failed"));
}

void Client::userReply(const std::string& message)
{
    std::string finalMessage = message + CRLF;
    if (send(_fd, finalMessage.c_str(), finalMessage.length(), 0) < 0)
        throw (std::runtime_error("writing message from client failed"));
}