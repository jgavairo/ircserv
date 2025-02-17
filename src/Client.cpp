#include "../inc/Client.hpp"
#include "../inc/Channel.hpp"

Client::Client(int fd) 
    : _fd(fd), _state(NOT_REGISTERED), _hostname("localhost") {}

Client::~Client() {}

//getters
int Client::getFd() const { return _fd; }
const std::string& Client::getHostname() const { return _hostname; }
const std::string& Client::getRealname() const { return _realname; }
const std::string& Client::getUsername() const { return _username; }
const std::string& Client::getNickname() const { return _nickname; }
ClientState Client::getState() const { return _state; }

//setters
void Client::setNickname(const std::string& nickname) { _nickname = nickname; }
void Client::setUsername(const std::string& username) { _username = username; }
void Client::setRealname(const std::string& realname) { _realname = realname; }
void Client::setState(ClientState state) { _state = state; }

void Client::reply(const std::string& reply)
{
    std::string serverName(SERVER_NAME);
    this->write(":" + serverName + " " + reply);
}

void Client::leaveChannel(Channel* channel)
{
    if (_channels.find(channel->getName()) != _channels.end())
    {
        _channels.erase(channel->getName());
        std::cout << "Client " << _nickname << " has left channel " << channel->getName() << std::endl;
    }
}
std::string Client::getPrefix() const 
{
    std::string username = _username.empty() ? "" : "!" + _username;
    std::string hostname = _hostname.empty() ? "" : "@" + _hostname;
	return (_nickname + username + hostname);
}
// EVENTUELLEMENT CHANGER LES FONCTIONNEMENT DES SEND() POUR LE SOCKET NON BLOQUANT
void Client::write(const std::string& message)
{
    std::string finalMessage = message + CRLF;
    std::cout << _nickname << message << std::endl;
    if (send(_fd, finalMessage.c_str(), finalMessage.length(), 0) < 0)
        throw (std::runtime_error("writing message from client failed"));
}

void Client::userReply(const std::string& message)
{
    std::string finalMessage = message + CRLF;
    if (send(_fd, finalMessage.c_str(), finalMessage.length(), 0) < 0)
        throw (std::runtime_error("writing message from client failed"));
}