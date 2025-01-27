#include "Server.hpp"

Server::Server(int argc, char**argv)
{
    initialisation(argc, argv);
}

Server::~Server()
{

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
        std::cerr << "Error accept() client connexion failed." << std::endl;
        return -1;
    }
    std::cout << "New client connected: " << client_socket << std::endl;

    pollfd client_poll_fd = {client_socket, POLLIN, 0};
    _fds.push_back(client_poll_fd);

    Client* new_client= new Client(client_socket);
    _clients[client_socket] = new_client;

    std::cout << "Client fd " << _clients[client_socket]->getFd() << " has been connected." << std::endl;

    return 0;
}

void Server::receiveNewSignal(size_t& i)
{
    char buffer[1024];
    ssize_t bytes_received = recv(_fds[i].fd, buffer, sizeof(buffer) - 1, 0);
    
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
        if (strcmp(buffer, "NICK"))
        {
            if (_commandsList._list.find("NICK") != _commandsList._list.end())
                _commandsList._list["NICK"]->use(*_clients[_fds[i].fd], "NEW NICKNAME");
        }
        std::cout << "message received! (from client " << _fds[i].fd << ") : " << buffer << std::endl;  
    }
}

void Server::run()
{
    pollfd server_poll_fd = {_fd, POLLIN, 0};
    _fds.push_back(server_poll_fd);
    _isRunning = true;
    while (true)
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
}