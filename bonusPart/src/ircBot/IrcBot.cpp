#include "../inc/IrcBot.hpp"

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
    std::string channel, command, noConstMessage;
    noConstMessage = message;
    size_t posHt, posPts;
    if (noConstMessage[0] == ':')
        noConstMessage.erase(0, 1);
    posHt = noConstMessage.find('#');
    posPts = noConstMessage.find(':');
    if (posHt != std::string::npos) 
    {
        size_t spaceAfterHash = noConstMessage.find(' ', posHt);
        if (spaceAfterHash != std::string::npos) 
        channel = noConstMessage.substr(posHt, spaceAfterHash - posHt);
    }
    
    if (posPts != std::string::npos) 
    {
        size_t spaceAfterColon = noConstMessage.find(' ', posPts);
        if (spaceAfterColon != std::string::npos)
        command = noConstMessage.substr(posPts, spaceAfterColon - posPts);
        else 
        command = noConstMessage.substr(posPts);
    }
    
    if (command[0] == ':')
    command.erase(0, 1);
    command.erase(command.find_last_not_of("\r\n") + 1);
    std::string output = (command + " " + channel + CRLF);
    send(_socket, output.c_str(), output.length(), 0);
}