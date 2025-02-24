#include "IrcBot.hpp"

IrcBot::IrcBot(int port, const std::string password) : _port(port), _password(password), _running(false), _socket(-1)
{

}

IrcBot::~IrcBot()
{
    if (_socket >= 0)
        close(_socket);
}

bool IrcBot::connect()
{
    _socket = socket(AF_INET, SOCK_STREAM, 0);
    if (_socket < 0)
    {
        std::cerr << "Error: Socket creation failed" << std::endl;
        return false;
    }

    struct sockaddr_in server_addr;
    std::memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(_port);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (::connect(_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0)
    {
        std::cerr << "Error: Connection failed" << std::endl;
        close(_socket);
        return false;
    }

    // Authentification du bot
    std::string auth = "PASS " + _password + "\r\n"
                      "NICK ircbot\r\n"
                      "USER ircbot 0 * :The real IRC bot\r\n";
    send(_socket, auth.c_str(), auth.length(), 0);

    fcntl(_socket, F_SETFL, O_NONBLOCK);
    return true;
}

void IrcBot::run()
{
    _running = true;
    char buffer[1024];

    while (_running)
    {
        ssize_t received = recv(_socket, buffer, sizeof(buffer) -1, 0);

        if (received > 0)
        {
            buffer[received] = '\0';
            handleCommand(std::string(buffer));
        }
        else if (received == 0)
        {
            std::cout << "Disconnected from server." << std::endl;
            break ;
        }
    }
}

void IrcBot::handleCommand(const std::string& message)
{
    // Implémenter la logique de réponse du bot ici
    std::cout << "Received: " << message << std::endl;
    size_t pos = message.find(":");
    if (pos == std::string::npos)
        return;
    std::string command = message.substr(pos + 1);
    pos = command.find(":");
    if (pos != std::string::npos)
        command = command.substr(pos + 1);
    if (!command.empty())
    {
        if (command[0] == ':')
            command.erase(0, 1);
        std::istringstream iss(command);
        std::string finalCommand;
        iss >> finalCommand;
        std::cout << "commande recue : [" << finalCommand << "]" << std::endl;
    }
}