#include "../inc/Client.hpp"
#include "../inc/Channel.hpp"

Client::Client(int fd) 
    : _fd(fd), _state(NOT_REGISTERED), _hostname("localhost"), _authenticated(false) {}

Client::~Client()
{
    if (_fd >= 0)
        close(_fd);
    
    std::vector<Channel*> channels_to_remove;
    for (std::map<std::string, Channel*>::iterator it = _channels.begin(); it != _channels.end(); ++it)
    {
        if (it->second)
            channels_to_remove.push_back(it->second);
    }
    
    for (size_t i = 0; i < channels_to_remove.size(); ++i)
    {
        if (channels_to_remove[i])
            channels_to_remove[i]->removeClient(this);
    }
    
    _channels.clear();
}

int Client::getFd() const { return _fd; }
const std::string& Client::getHostname() const { return _hostname; }
const std::string& Client::getRealname() const { return _realname; }
const std::string& Client::getPassword() const { return _password; }
const std::string& Client::getUsername() const { return _username; }
const std::string& Client::getWelcomeMessge() const { return _welcomeMessage; }
const std::string& Client::getNickname() const { return _nickname; }
ClientState Client::getState() const { return _state; }
std::string& Client::getBuffer() { return _buffer; }

void Client::setNickname(const std::string& nickname) { _nickname = nickname; }
void Client::setUsername(const std::string& username) { _username = username; }
void Client::setPassword(const std::string& password) { _password = password; }
void Client::setRealname(const std::string& realname) { _realname = realname; }
void Client::setWelcomeMessage(const std::string& message) { _welcomeMessage = message; }
void Client::setState(ClientState state) { _state = state; }
void Client::appendToBuffer(const std::string& data) { _buffer += data; }

bool Client::isAuthenticated() { return _authenticated; }
void Client::clearBuffer() { _buffer.clear(); }

void Client::clearWelcomeMessage() {_welcomeMessage.clear();}

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
    if (send(_fd, finalMessage.c_str(), finalMessage.length(), 0) < 0)
        throw (std::runtime_error("writing message from client failed"));
}

void Client::userReply(const std::string& message)
{
    std::string finalMessage = message + CRLF;
    if (send(_fd, finalMessage.c_str(), finalMessage.length(), 0) < 0)
        throw (std::runtime_error("writing message from client failed"));
}