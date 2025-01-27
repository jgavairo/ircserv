#include "../../inc/commands/Nick.hpp"

Nick::Nick() : _name("NICK") {}

Nick::~Nick() {}

void Nick::use(Client& client, std::string nickname)
{
    client.setNickname(nickname);
    std::cout << "your nickname is now: " << nickname << std::endl;
}
