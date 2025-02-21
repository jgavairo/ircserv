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
#define ERR_NOTONCHANNEL(client, channel) "442 " + client + " " + channel + " :You're not on that channel"
#define ERR_NORECIPIENT(client) "411 " + client + " :No recipient given (PRIVMSG)"
#define ERR_NOTEXTTOSEND(client) "412 " + client + " :No text to send"
#define ERR_CANNOTSENDTOCHAN(client, channel) "404 " + client + " " + channel + " :Cannot send to channel"
#define ERR_NOSUCHNICK(client, nick) "401 " + client + " " + nick + " :No such nick/channel"

#define RPL_NOTOPIC(client, channel) "331 " + client + " " + channel + " :No topic is set"
#define RPL_TOPIC(client, channel, topic) "332 " + client + " " + channel + " :" + topic
#define ERR_CHANOPRIVSNEEDED(client, channel) "482 " + client + " " + channel + " :You're not channel operator"

#define ERR_PASSWDMISMATCH(client) "464 " + client + " :Password incorrect"

#define ERR_NONICKNAMEGIVEN(client) "431 " + client + " :No nickname given"
#define ERR_ERRONEUSNICKNAME(client, nick) "432 " + client + " " + nick + " :Erroneous nickname"

#define RPL_NICK(oldPrefix, newNick) ":" + oldPrefix + " NICK :" + newNick
#define RPL_JOIN(prefix, channel) ":" + prefix + " JOIN :" + channel
#define RPL_WELCOME(client) "001 " + client + " :Welcome to the Internet Relay Network ft_irc " + client

#define PART_MESSAGE(clientPrefix, channel, reason) ":" + clientPrefix + " PART " + channel + (reason.empty() ? "" : " :" + reason)

#define RPL_NAMREPLY(client, channel, namesList) "353 " + client + " = " + channel + " :" + namesList
#define RPL_ENDOFNAMES(client, channel) "366 " + client + " " + channel + " :End of /NAMES list."

#define RPL_INVITING(nickname, channel, inviter) "341 " + nickname + " " + channel + " " + inviter

#define RPL_LISTSTART(client) "321 " + client + " Channel :Users Name"
#define RPL_LIST(client, channel, users, topic) "322 " + client + " " + channel + " " + users + " :" + topic
#define RPL_LISTEND(client) "323 " + client + " :End of /LIST"
#define ERR_USERNOTINCHANNEL(client, user, channel) "441 " + client + " " + user + " " + channel + " :They aren't on that channel"

//FOR MODE
#define RPL_CHANNELMODEIS(nickname, channel, modes) "324 " + nickname + " " + channel + " " + modes
#define ERR_UNKNOWNMODE(nick, mode) (":" + nick + " 472 " + mode + " :is unknown mode char to me")
#define RPL_KICK(kicker, channel, user, comment) ":" + kicker + " KICK " + channel + " " + user + " :" + comment

#define ERR_INVITEONLYCHAN(nick, channel) (":" + nick + " 473 " + channel + " :Cannot join channel (+i)")
#define ERR_BADCHANNELKEY(nick, channel) (":" + nick + " 475 " + channel + " :Cannot join channel (+k)")
#define ERR_CHANNELISFULL(nick, channel) (":" + nick + " 471 " + channel + " :Cannot join channel (+l)")
#define ERR_CANNOTSETLIMIT(nick, channel) (":" + nick + " 471 " + channel + " :Cannot set limit lower than current users")


// Définition des messages de broadcast
#define NOTICE_INVITE_ONLY_SET(prefix, channel) (":" + prefix + " NOTICE " + channel + " :Mode +i has been set")
#define NOTICE_INVITE_ONLY_UNSET(prefix, channel) (":" + prefix + " NOTICE " + channel + " :Mode -i has been unset")
#define NOTICE_TOPIC_RESTRICTED_SET(prefix, channel) (":" + prefix + " NOTICE " + channel + " :Mode +t has been set")
#define NOTICE_TOPIC_RESTRICTED_UNSET(prefix, channel) (":" + prefix + " NOTICE " + channel + " :Mode -t has been unset")
#define NOTICE_PASSWORD_SET(prefix, channel) (":" + prefix + " NOTICE " + channel + " :Mode +k has been set")
#define NOTICE_PASSWORD_UNSET(prefix, channel) (":" + prefix + " NOTICE " + channel + " :Mode -k has been unset")
#define NOTICE_OPERATOR_ADDED(prefix, channel, param) (":" + prefix + " NOTICE " + channel + " :Mode +o " + param + " has been set")
#define NOTICE_OPERATOR_REMOVED(prefix, channel, param) (":" + prefix + " NOTICE " + channel + " :Mode -o " + param + " has been unset")
#define NOTICE_USER_LIMIT_SET(prefix, channel, param) (":" + prefix + " NOTICE " + channel + " :Mode +l " + param + " has been set")
#define NOTICE_USER_LIMIT_UNSET(prefix, channel) (":" + prefix + " NOTICE " + channel + " :Mode -l has been unset")

#define MAX_CLIENTS 5
