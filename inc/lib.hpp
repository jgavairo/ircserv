#pragma once

#include <sys/socket.h>
#include <iostream>
#include <sstream>
#include <string>
#include <cerrno>
#include <cstring>
#include <cstdlib>
#include <netinet/in.h>
#include <unistd.h>
#include <map>
#include <vector>
#include <iterator>
#include <fcntl.h>
#include <set>
#include <poll.h>

#define SERVER_NAME "my_irc_network"
#define CRLF "\r\n"

#define ERR_NEEDMOREPARAMS(client, command) "461 " + client + " " + command + " :Not enough parameters"
#define ERR_ALREADYREGISTERED(client) "462 " + client + " :You may not reregister"
#define ERR_NOSUCHCHANNEL(client, channel) "403 " + client + " " + channel + " :No such channel"
#define ERR_NOTREGISTERED() "451 :You have not registered"
#define ERR_NICKNAMEINUSE(nick) "433 " + nick + " " + nick + " nickname is already in use"
#define RPL_NICK(oldPrefix, newNick) ":" + oldPrefix + " NICK :" + newNick

#define RPL_JOIN(prefix, channel) ":" + prefix + " JOIN :" + channel
#define RPL_WELCOME(client) "001 " + client + " :Welcome to the Internet Relay Network ft_irc " + client

#define MAX_CLIENTS 5
