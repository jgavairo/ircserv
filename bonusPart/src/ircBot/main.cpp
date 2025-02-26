#include "../../inc/IrcBot.hpp"

int main(int argc, char** argv)
{
    if (argc != 3)
    {
        std::cerr << "Usage: " << argv[0] << " <port> <password>" << std::endl;
        return 1;
    }

    IrcBot bot(atoi(argv[1]), argv[2]);
    
    if (!bot.connect())
    {
        std::cerr << "Failed to connect to server" << std::endl;
        return 1;
    }

    std::cout << "Bot connected to server" << std::endl;
    bot.run();

    return 0;
}