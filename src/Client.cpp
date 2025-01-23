#include "Client.hpp"

Client::Client(int fd) 
    : _fd(fd), _authenticated(false) {}

Client::~Client() {}

//getters
int Client::getFd() const { return _fd; }
const std::string& Client::getHostname() const { return _hostname; }
const std::string& Client::getRealname() const { return _realname; }
const std::string& Client::getNickname() const { return _nickname; }

//setters
void Client::setNickname(const std::string& nickname) { _nickname = nickname; }
void Client::setUsername(const std::string& username) { _username = username; }
void Client::setRealname(const std::string& realname) { _realname = realname; }

//function for approve auth
void Client::authentificate() { _authenticated = true; }

//getter state auth
bool Client::isAuthentificated() const { return _authenticated; }