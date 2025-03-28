#include "../../inc/commands/CommandsList.hpp"
#include "../../inc/commands/Nick.hpp"
#include "../../inc/commands/User.hpp"
#include "../../inc/commands/Join.hpp"
#include "../../inc/commands/Privmsg.hpp"
#include "../../inc/commands/Notice.hpp"
#include "../../inc/commands/Pass.hpp"
#include "../../inc/commands/Quit.hpp"
#include "../../inc/commands/List.hpp"
#include "../../inc/commands/Kick.hpp"
#include "../../inc/commands/Topic.hpp"
#include "../../inc/commands/Invite.hpp"
#include "../../inc/commands/Part.hpp"
#include "../../inc/commands/Mode.hpp"


CommandsList::CommandsList()
{
    _list["NICK"] = new Nick();
    _list["USER"] = new User();
    _list["JOIN"] = new Join();
    _list["PRIVMSG"] = new Privmsg();
    _list["NOTICE"] = new Notice();
    _list["PASS"] = new Pass();
    _list["QUIT"] = new Quit();
    _list["TOPIC"] = new Topic();
    _list["PART"] = new Part();
    _list["KICK"] = new Kick();
    _list["INVITE"] = new Invite();
    _list["LIST"] = new List();
    _list["MODE"] = new Mode();
}

CommandsList::~CommandsList()
{
    for (std::map<std::string, ACommand*>::iterator it = _list.begin(); it != _list.end(); ++it)
    {
        delete it->second;
    }
    _list.clear();
}
