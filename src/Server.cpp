#include "Server.hpp"

Server::Server(int argc, char**argv)
{
    std::cout << "Server initialisation..." << std::endl;
    initialisation(argc, argv);
    std::cout << "Server initialized!" << std::endl;
}

Server::~Server()
{

}

void Server::initialisation(int argc, char**argv)
{
    if (argc != 3)
        throw(std::invalid_argument("Please type: './ircserv <port> <password>'"));
    _isRunning = false; 
    _port = std::atoi(argv[1]);
    //ajouter des verifications ??
    _password = argv[2];
    _fd = socket(AF_INET, SOCK_STREAM, 0);
    if (_fd < 0)
        throw(std::runtime_error("socket failed"));

    //initialisation adress
    std::memset(&_server_adress, 0, sizeof(_server_adress));
    _server_adress.sin_family = AF_INET;
    _server_adress.sin_addr.s_addr = htonl(INADDR_ANY);
    _server_adress.sin_port = htons(_port);

    //associer le socket a la structure stockant l'adresse IP et le port
    if (bind(_fd, (sockaddr*)&_server_adress, sizeof(_server_adress)) < 0)
        throw (std::runtime_error("bind failed"));
    
        //mettre le socket en mode ecoute
    if (listen(_fd, MAX_CLIENTS) < 0)
        throw (std::runtime_error("listen failed"));
    std::cout << "Server listenning on port " << _port << std::endl;
}

void Server::run()
{
    pollfd server_poll_fd = {_fd, POLLIN, 0};
    _fds.push_back(server_poll_fd);
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
                sockaddr_in client_adress;
                socklen_t client_len = sizeof(client_adress);

                int client_socket = accept(_fd, (sockaddr*)&client_adress, &client_len);
                if (client_socket < 0)
                {
                    std::cerr << "Error accept() client connexion failed." << std::endl;
                    continue ;
                }
                std::cout << "New client connected: " << client_socket << std::endl;

                pollfd client_poll_fd = {client_socket, POLLIN, 0};
                _fds.push_back(client_poll_fd);

                Client* new_client= new Client(client_socket);
                _clients[client_socket] = new_client;

                std::cout << "Client fd " << _clients[client_socket]->getFd() << " has been connected." << std::endl;
            }
            //messages entrants
            else if (_fds[i].revents & POLLIN)
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
                    std::cout << "message received! (from client " << _fds[i].fd << ") : " << buffer << std::endl;  
                }
            }
        }
    }
}