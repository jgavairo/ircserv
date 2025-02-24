#include "Server.hpp"
#include "../inc/Channel.hpp"

Server* Server::_instance = NULL;

Server::Server(int argc, char**argv)
{
    initialisation(argc, argv);
}

void Server::handleSignal(int signum)
{
    if (signum == SIGINT)
    {
        std::cout << "\nReceived SIGINT signal. Shutting down server..." << std::endl;
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
    // Fermer le socket serveur
    if (_fd >= 0)
        close(_fd);

    // Supprimer les clients
    for (std::map<int, Client*>::iterator it = _clients.begin(); it != _clients.end(); ++it) {
        if (it->second) {
            close(it->second->getFd());
            delete it->second;
        }
    }
    _clients.clear();

    // Supprimer les channels
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
    // Recherchez le canal par son nom
    std::map<std::string, Channel*>::iterator it = _channels.find(name);
    if (it != _channels.end())
    {
        // Vérifiez si le canal est vide
        if (it->second->isEmpty()) {
            // Supprimez l'entrée du canal de la liste
            _channels.erase(it);
        }
    }
}

void Server::initialisation(int argc, char**argv)
{
    _name = SERVER_NAME;
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
    //initialisation adress
    std::memset(&_server_adress, 0, sizeof(_server_adress));
    _server_adress.sin_family = AF_INET;
    _server_adress.sin_addr.s_addr = htonl(INADDR_ANY);
    _server_adress.sin_port = htons(_port);

    //associer le socket a la structure stockant l'adresse IP et le port
    if (bind(_fd, (sockaddr*)&_server_adress, sizeof(_server_adress)) < 0)
        throw (std::runtime_error("Initialisation: FAIL (bind failed)"));
    
    //mettre le socket en mode ecoute
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
    std::cout << "New client connected: " << client_socket << std::endl;

    std::string initialResponse = ":irc.example.com NOTICE * :Welcome to My IRC Server!\r\n";
    ssize_t bytes_sent = send(client_socket, initialResponse.c_str(), initialResponse.size(), 0);
    if (bytes_sent < 0 && (errno == EWOULDBLOCK || errno == EAGAIN)) 
    {
        pollfd client_poll_fd = {client_socket, POLLIN | POLLOUT, 0};  // Ajout de POLLOUT
        _fds.push_back(client_poll_fd);
    }
    else 
    {
        pollfd client_poll_fd = {client_socket, POLLIN, 0};
        _fds.push_back(client_poll_fd);
    }
    Client* new_client= new Client(client_socket);
    _clients[client_socket] = new_client;

    std::cout << "Client fd " << _clients[client_socket]->getFd() << " has been connected." << std::endl;

    return 0;
}

void Server::receiveNewSignal(size_t& i)
{
    char buffer[1024];
    ssize_t bytes_received = recv(_fds[i].fd, buffer, sizeof(buffer) - 1, 0);
    int saved_errno = errno;

    if (bytes_received == 0)
    {
        //deconnexion du client
        std::cout << "client has disconnected. fd: " << _fds[i].fd << std::endl;
        close(_fds[i].fd);
        delete _clients[_fds[i].fd];
        _fds.erase(_fds.begin() + i);
        i--;
    }
    else if (bytes_received < 0)
    {
        //erreur
        if (saved_errno == EWOULDBLOCK || errno == EAGAIN)
            return ;
        std::cout << "Error: impossible to reading from client, " << strerror(errno) << std::endl;
        close(_fds[i].fd);
        delete _clients[_fds[i].fd];
        _fds.erase(_fds.begin() + i);
        i--;
    }
    else
    {
        //OK
        buffer[bytes_received] = '\0';
        std::string input(buffer);

        std::cout << "New message received from fd[" << _fds[i].fd << "] : {" << input << "}" << std::endl;
        std::vector<std::string> commandLines = _parser.splitByCRLF(input);
        handleCommands(_clients[_fds[i].fd], commandLines);
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
            //nouveaux clients
            if (_fds[i].fd == _fd && (_fds[i].revents & POLLIN))
            {
                if (addNewClient() == -1)
                    continue;
            }
            //messages entrants
            else if (_fds[i].revents & POLLIN)
                receiveNewSignal(i);
        }
    }
        // Cleanup avant de quitter
        std::cout << "Closing all connections..." << std::endl;
    
        // Fermer toutes les connexions client avant de terminer
        for (size_t i = 1; i < _fds.size(); ++i)
        {
            if (_fds[i].fd >= 0)
            {
                close(_fds[i].fd);
                if (_clients.find(_fds[i].fd) != _clients.end())
                {
                    delete _clients[_fds[i].fd];
                    _clients.erase(_fds[i].fd);
                }
            }
        }
        _fds.clear();
        std::cout << "Server shutdown complete." << std::endl;
}