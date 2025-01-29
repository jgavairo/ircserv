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
#include <set>
#include <poll.h>

#define SERVER_NAME "my_irc_network"
#define CRLF "\r\n"

#define ERR_NEEDMOREPARAMS(client, command) "461 " + client + " " + command + " :Not enough parameters"
#define ERR_ALREADYREGISTERED(client) "462 " + client + " :You may not reregister"

#define MAX_CLIENTS 5
