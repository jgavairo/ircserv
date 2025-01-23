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
        int poll_count = poll(_fds.data(), _fds.size(), -1);
        if (poll_count < 0)
            std::cerr << "Error poll() client connexion failed." << std::endl;
        
        if (_fds[0].revents & POLLIN)
        {
            sockaddr_in client_adress;
            socklen_t client_len = sizeof(client_adress);

            int client_socket = accept(_fd, (sockaddr*)&client_adress, &client_len);
            if (client_socket < 0)
                std::cerr << "Error accept() client connexion failed." << std::endl;
            std::cout << "New client connected: " << client_socket << std::endl;
            pollfd client_poll_fd = {client_socket, POLLIN, 0};
            _fds.push_back(client_poll_fd);
            //add client a _clients
            Client new_client(client_socket);
            _clients[client_socket] = &new_client;
            std::cout << "Client fd " << _clients[client_socket]->getFd() << " has been connected." << std::endl;
        }
    }
}