#include "../../inc/commands/BotPunchline.hpp"
#include "../../inc/Server.hpp"
#include "../../inc/Channel.hpp"
#include <ctime>

BotPunchline::BotPunchline()
{
    _punchlines["\"Dites à Donald qu’il s'Trump\""] = "Lle-pier,  Le Cafard.";
    _punchlines["\"J'arrive à balle, gé-char, donc ça me gratte vers le zizi\""] = "Qordoux, Le samourai d'Auxerre.";
    _punchlines["\"Je suis dans une sacrée salade, y m'faut deux avocats\""] = "Lmattern, Sergent-Chef.";
    _punchlines["\"Frère si je sors la AK, je sais qu'ils feront tous caca\""] = "Fprevot, 4-Steaks.";
    _punchlines["\"T'as l'impression que je scratche quand je gratte mes deux grosses couilles crépues\""] = "Maeferre, Jamais la.";
    _punchlines["\"J’ai trop d’inspi, ça sort de mon corps comme si j’avais lâché une crotte\""] = "Abolea, Bleu metal.";
}

BotPunchline::~BotPunchline() {}

void BotPunchline::execute(Client* client, std::string arguments)
{
    if (client->getNickname() == "ircbot")
    {
        std::string channel = arguments.erase(0, arguments.find_first_not_of(" \t"));
        srand(time(0));
        std::map<std::string, std::string>::iterator it = _punchlines.begin();
        int randomIndex = rand() % _punchlines.size();
        for (int i = 0; i < randomIndex; ++i)
            ++it;
        std::string punchline = it->first, artist = it->second, prefix;
        client->_channels[channel]->broadcast(NOTICE_BOT_PUNCHLINE(client->getPrefix(), channel, punchline, artist), client);
    }
}