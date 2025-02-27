#include "../../inc/commands/Pass.hpp"
#include "../../inc/Server.hpp"
#include "../../inc/Channel.hpp"

Pass::Pass() {}

Pass::~Pass() {}

void Pass::execute(Client* client, std::string arguments)
{

    if (arguments.empty())
    {
        (client->reply(ERR_NEEDMOREPARAMS(client->getNickname(), "PASS")));
        return ;
    }
    else
    {
        if (arguments[0] == ' ')
            arguments.erase(0, 1);
        client->setPassword(arguments);
    }

    Server* server = Server::getInstance();
    
    if (client->getPassword() == server->getPassword())
    {
        client->userReply(NOTICE_PASS_OK());
        client->setAuth(true);
    }
    else
    {
        client->setAuth(false);
        client->reply(ERR_PASSWDMISMATCH(client->getNickname()));
    }

}