#include "../../inc/commands/BotQuizz.hpp"
#include "../../inc/Server.hpp"
#include "../../inc/Channel.hpp"
#include <ctime>

BotQuizz::BotQuizz() {}

BotQuizz::~BotQuizz() {}

void BotQuizz::execute(Client* client, std::string arguments)
{
    if (client->getNickname() == "ircbot")
    {
        std::string channel = arguments.erase(0, arguments.find_first_not_of(" \t"));
        static std::map<std::string, std::string> activeQuizzes;
        static std::map<std::string, std::string> _quizz;
        _quizz["Jul"] = "Je fais des hits, je fais des tubes, je fais des classiques, je fais des trucs.";
        _quizz["SDM"] = "J'ai pas changé, j'ai juste grandi, j'ai pris du recul, j'ai pris des risques.";
        _quizz["Booba"] = "J'ai vu des choses que les grands n'ont pas vues, j'ai fait des choses que les grands n'ont pas faites.";

        if (activeQuizzes.find(channel) == activeQuizzes.end())
        {
            srand(time(0));
            std::map<std::string, std::string>::iterator it = _quizz.begin();
            int randomIndex = rand() % _quizz.size();
            for (int i = 0; i < randomIndex; ++i)
            {
                ++it;
            }
            activeQuizzes[channel] = it->first;

            std::string message = ":" + client->getPrefix() + " NOTICE " + channel + " :" + it->second;
            client->_channels[channel]->broadcast(message, client);
        }
        else
        {
            std::string correctAnswer = activeQuizzes[channel];
            if (arguments == correctAnswer)
            {
                std::string message = ":" + client->getPrefix() + " NOTICE " + channel + " :Correct! The artist is " + correctAnswer;
                client->_channels[channel]->broadcast(message, client);
                activeQuizzes.erase(channel);
            }
            else
            {
                std::string message = ":" + client->getPrefix() + " NOTICE " + channel + " :Incorrect! Try again.";
                client->_channels[channel]->broadcast(message, client);
            }
        }
    }
}