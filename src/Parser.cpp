#include "Parser.hpp"

Parser::Parser() {}

Parser::~Parser() {}

// VERIFICATION DES PARAMETRES, 2 ARGUMENTS / UN PORT CONTENANT SEULEMENT DES CHIFFRES / RESPECT DES LIMITES
void Parser::parametersChecker(int argc, char** argv, int& _port)
{
    if (argc != 3)
        throw(std::invalid_argument("[ERROR] Please type: './ircserv <port> <password>'"));
    for (size_t i = 0; argv[1][i]; i++)
    {
        if (!isdigit(argv[1][i]))
            throw (std::invalid_argument("[ERROR] The port must be a number."));
    }
    _port = std::atoi(argv[1]);
    if (_port >= 65535)
        throw (std::invalid_argument("[ERROR] This port doesn't exist. (must be beetween 0 and 65535)"));

}