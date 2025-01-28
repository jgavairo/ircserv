#include "../../inc/commands/CommandsList.hpp"

CommandsList::CommandsList()
{
    _list["NICK"] = new Nick();

    //ADD ALL COMMANDES
}

CommandsList::~CommandsList() {}



