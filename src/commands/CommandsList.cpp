#include "../../inc/commands/CommandsList.hpp"
#include "../../inc/commands/Nick.hpp"
#include "../../inc/commands/User.hpp"
#include "../../inc/commands/Join.hpp"

CommandsList::CommandsList()
{
    _list["NICK"] = new Nick();
    _list["USER"] = new User();
    _list["JOIN"] = new Join();

    //ADD ALL COMMANDES
}

CommandsList::~CommandsList() {}



