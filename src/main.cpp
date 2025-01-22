#include <sys/socket.h>
#include <iostream>
#include <cerrno>
#include <cstring>
#include <cstdlib>
#include <netinet/in.h>
#include <unistd.h>

int main (int argc, char**argv)
{
    if (argc != 3)
        return (std::cerr << "ERROR: invalids arguments, please type './ircserv <ip> <port>'" << std::endl, -1);

    int port = atoi(argv[1]);
    std::string password = argv[2];

    //creation du socket
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0)
        return (std::cerr << "server_socket creation failed: " << strerror(errno) << std::endl, -1);

    //configuration de l'adresse IP et du port
    sockaddr_in server_adress;
    std::memset(&server_adress, 0, sizeof(server_adress));
    server_adress.sin_family = AF_INET;
    server_adress.sin_addr.s_addr = htonl(INADDR_ANY);
    server_adress.sin_port = htons(port);

    //associer le socket a la structure stockant l'adresse IP et le port
    if (bind(server_socket, (sockaddr*)&server_adress, sizeof(server_adress)) < 0)
        return (std::cerr << "bind server failed: " << strerror(errno) << std::endl, -1);

    //mettre le socket en mode ecoute
    if (listen(server_socket, 5) < 0)
        return (std::cerr << "listen server failed: " << strerror(errno) << std::endl, close(server_socket), -1);
    std::cout << "Server listenning on port " << port << std::endl;

    //reception et acceptation des connexions entrantes
    sockaddr_in client_adress;
    socklen_t client_len = sizeof(client_adress);

    while (true)
    {
        //attente de connextions..

        //connexion entrante:
        int client_socket = accept(server_socket, (sockaddr*)&client_adress, &client_len);
        if (client_socket < 0)
        {
            //connexion impossible
            std::cerr << "accept client failed: " << strerror(errno) << std::endl;
            continue ;
        }
        //connexion reussie:
        std::cout << "new client connected!" << std::endl;
        
    }

    close(server_socket);
    

}