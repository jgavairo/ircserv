#include "../../inc/commands/CommandsList.hpp"

CommandsList::CommandsList()
{
    _list["NICK"] = new Nick();
    _list["USER"] = new User();

    //ADD ALL COMMANDES
}

CommandsList::~CommandsList() {}



