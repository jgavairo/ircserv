#include "Server.hpp"
#include "../inc/Channel.hpp"

Server* Server::_instance = NULL;

Server::Server(int argc, char**argv) : _isRunning(false)
{
    initialisation(argc, argv);
}

void Server::handleSignal(int signum)
{
    if (signum == SIGINT)
    {
        std::cout << "Received SIGINT signal. Shutting down server..." << std::endl;
        _isRunning = false;
    }
}

void Server::signalCallback(int signum)
{
    if (_instance)
        _instance->handleSignal(signum);
}

Server::~Server()
{
    if (_fd >= 0)
        close(_fd);
    for (std::map<int, Client*>::iterator it = _clients.begin(); it != _clients.end(); ++it) {
        if (it->second) 
        {
            close(it->second->getFd());
            delete it->second;
        }
    }
    _clients.clear();
    for (std::map<std::string, Channel*>::iterator it = _channels.begin(); it != _channels.end(); ++it) 
    {
        if (it->second)
            delete it->second;
    }
    _channels.clear();
    _instance = NULL;

}

std::map<std::string, Channel*>& Server::getChannels()
{
    return _channels;
}

Server* Server::getInstance(int argc, char**argv)
{
    if (_instance == NULL)
    {
        if (argc == 0 || argv == NULL)
            throw std::runtime_error("Server must be initialized with arguments first.");
        _instance = new Server(argc, argv);
    }
    return _instance;
}

const std::string& Server::getPassword() const { return _password; }

void Server::removeEmptyChannel(const std::string& name) 
{
    std::map<std::string, Channel*>::iterator it = _channels.find(name);
    if (it != _channels.end())
    {
        if (it->second->isEmpty()) 
        {
            delete it->second;
            _channels.erase(it);
        }
    }
}

void Server::initialisation(int argc, char**argv)
{
    _parser.parametersChecker(argc, argv, _port);
    std::cout << "Server initialisation..." << std::endl;
    _isRunning = false;
    _password = argv[2];
    _fd = socket(AF_INET, SOCK_STREAM, 0);
    if (_fd < 0)
        throw(std::runtime_error("Initialisation: FAIL (socket failed)"));

    int opt = 1;
    if (setsockopt(_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
    {
        close(_fd);
        throw std::runtime_error("Initialisation: FAIL (setsockopt failed)");
    }
    fcntl(_fd, F_SETFL, O_NONBLOCK);

    std::memset(&_server_adress, 0, sizeof(_server_adress));
    _server_adress.sin_family = AF_INET;
    _server_adress.sin_addr.s_addr = htonl(INADDR_ANY);
    _server_adress.sin_port = htons(_port);

    if (bind(_fd, (sockaddr*)&_server_adress, sizeof(_server_adress)) < 0)
        throw (std::runtime_error("Initialisation: FAIL (bind failed)"));
    
    if (listen(_fd, MAX_CLIENTS) < 0)
        throw (std::runtime_error("Initialisation: FAIL (listen failed)"));
    std::cout << "Initialisation: OK" << std::endl;
    std::cout << "Server listenning on port " << _port << std::endl;
}

int Server::addNewClient()
{
    sockaddr_in client_adress;
    socklen_t client_len = sizeof(client_adress);

    int client_socket = accept(_fd, (sockaddr*)&client_adress, &client_len);
    if (client_socket < 0)
    {
        if (errno != EWOULDBLOCK && errno != EAGAIN)
            std::cerr << "Error accept() client connexion failed." << std::endl;
        return -1;
    }
    fcntl(client_socket, F_SETFL, O_NONBLOCK);

    Client* new_client= new Client(client_socket);
    _clients[client_socket] = new_client;
    
    std::string initialResponse = ":irc.example.com NOTICE * :Welcome to My IRC Server!\r\n";
    new_client->setWelcomeMessage(initialResponse);

    pollfd client_poll_fd = {client_socket, POLLIN | POLLOUT, 0};
    _fds.push_back(client_poll_fd);

    std::cout << "Client fd " << _clients[client_socket]->getFd() << " has been connected." << std::endl;

    return 0;
}

void Server::receiveNewSignal(size_t& i)
{
    char buffer[1024];
    ssize_t bytes_received = recv(_fds[i].fd, buffer, sizeof(buffer) - 1, 0);
    int saved_errno = errno;

    if (bytes_received <= 0)
    {
        if (bytes_received == 0)
            std::cout << "client has disconnected. fd: " << _fds[i].fd << std::endl;
        else if (saved_errno == EWOULDBLOCK || saved_errno == EAGAIN)
            return;
        else
            std::cout << "Error: impossible to reading from client, " << strerror(saved_errno) << std::endl;

        close(_fds[i].fd);
        if (_clients.find(_fds[i].fd) != _clients.end()) 
        {
            delete _clients[_fds[i].fd];
            _clients.erase(_fds[i].fd);
        }
        _fds.erase(_fds.begin() + i);
        i--;
        return;
    }
    buffer[bytes_received] = '\0';
    std::string input(buffer);
    
    if (_clients.find(_fds[i].fd) == _clients.end())
        return;
    
    Client* client = _clients[_fds[i].fd];
    
    client->appendToBuffer(input);

    std::string& clientBuffer = client->getBuffer();
    
    size_t pos = clientBuffer.find("\r\n");
    std::vector<std::string> commandsToProcess;
    
    while (pos != std::string::npos) 
    {
        std::string completeCommand = clientBuffer.substr(0, pos);
        if (!completeCommand.empty())
            commandsToProcess.push_back(completeCommand);
        
        clientBuffer = clientBuffer.substr(pos + 2);
        
        pos = clientBuffer.find("\r\n");
    }
    pos = clientBuffer.find("\n");
    while (pos != std::string::npos) 
    {
        if (pos == 0 || clientBuffer[pos - 1] != '\r')
        {
            std::string completeCommand = clientBuffer.substr(0, pos);
            if (!completeCommand.empty())
                commandsToProcess.push_back(completeCommand);
        }
        clientBuffer = clientBuffer.substr(pos + 1);
        pos = clientBuffer.find("\n");
    }
    
    if (!commandsToProcess.empty())
    {
        handleCommands(client, commandsToProcess);
        if (_clients.find(_fds[i].fd) == _clients.end())
            return;
    }
    
    if (clientBuffer.size() > 4096)
    {
        std::cout << "Warning: Client buffer overflow, truncating. fd: " << _fds[i].fd << std::endl;
        clientBuffer = clientBuffer.substr(0, 4096);
    }
}

void Server::handleCommands(Client* client, std::vector<std::string> commandLines)
{
    for (size_t i = 0; i < commandLines.size(); i++)
    {
        std::string commandName, arguments;
        std::istringstream iss(commandLines[i]);
        iss >> commandName;
        if (_commandsList._list.find(commandName) != _commandsList._list.end())
        {
            std::getline(iss, arguments);
            _commandsList._list[commandName]->execute(client, arguments);
        }
    }
}

void Server::updateNickInChannels(const std::string& old_, const std::string& new_)
{
    for (std::map<std::string, Channel*>::const_iterator it = _channels.begin(); it != _channels.end(); ++it)
        it->second->updateNickname(old_, new_);
}

std::map<int, Client*>& Server::getClients()
{
    return _clients;
}

void Server::run()
{
    pollfd server_poll_fd = {_fd, POLLIN, 0};
    _fds.push_back(server_poll_fd);
    _isRunning = true;
    std::signal(SIGINT, Server::signalCallback);
    while (_isRunning)
    {
        int poll_count = poll(_fds.data(), _fds.size(), 100);
        if (poll_count < 0)
        {
            std::cerr << "Error poll() client connexion failed." << std::endl;
            continue ;
        }
        for (size_t i = 0; i < _fds.size(); ++i)
        {
            if (_fds[i].fd == _fd && (_fds[i].revents & POLLIN))
            {
                if (addNewClient() == -1)
                    continue;
            }
            else if (_fds[i].revents & POLLIN)
                receiveNewSignal(i);
            else if (_fds[i].revents & POLLOUT)
            {
                if (_clients.find(_fds[i].fd) != _clients.end())
                {
                    Client* client = _clients[_fds[i].fd];
                    std::string welcomeMessage = client->getWelcomeMessge();
                    if (!welcomeMessage.empty())
                    {
                        send(_fds[i].fd, welcomeMessage.c_str(), welcomeMessage.size(), 0);
                        client->clearWelcomeMessage();
                        _fds[i].events = POLLIN;
                    }
                }
            }
        }
    }
    std::cout << "Closing all connections..." << std::endl;

    for (size_t i = 1; i < _fds.size(); ++i)
    {
        if (_fds[i].fd >= 0)
            close(_fds[i].fd);
    }
    _fds.clear();
    std::cout << "Server shutdown complete." << std::endl;
}