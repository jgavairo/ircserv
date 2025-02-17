#include "../../inc/commands/CommandsList.hpp"
#include "../../inc/commands/Nick.hpp"
#include "../../inc/commands/User.hpp"
#include "../../inc/commands/Join.hpp"
#include "../../inc/commands/Privmsg.hpp"
#include "../../inc/commands/Notice.hpp"

CommandsList::CommandsList()
{
    _list["NICK"] = new Nick();
    _list["USER"] = new User();
    _list["JOIN"] = new Join();
    _list["PRIVMSG"] = new Privmsg();
    _list["NOTICE"] = new Notice();

    //ADD ALL COMMANDES
}

CommandsList::~CommandsList() {}



