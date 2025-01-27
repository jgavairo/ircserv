#include "../../inc/commands/CommandsList.hpp"

CommandsList::CommandsList()
{
    ACommand* command = new Nick();
    _list[command->_name] = command;

    //ADD ALL COMMANDES
}

CommandsList::~CommandsList() {}



