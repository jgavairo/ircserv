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
#include <arpa/inet.h>
#include <csignal>
#include <set>
#include <poll.h>

#define SERVER_NAME "my_irc_network"
#define MAX_CLIENTS 5
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
#define ERR_CHANOPRIVSNEEDED(client, channel) "482 " + client + " " + channel + " :You're not channel operator"
#define ERR_PASSWDMISMATCH(client) "464 " + client + " :Password incorrect"
#define ERR_NONICKNAMEGIVEN(client) "431 " + client + " :No nickname given"
#define ERR_ERRONEUSNICKNAME(nick) "432 " + nick + " :Erroneous nickname"
#define ERR_USERNOTINCHANNEL(client, user, channel) "441 " + client + " " + user + " " + channel + " :They aren't on that channel"
#define ERR_UNKNOWNMODE(nick, mode) (":" + nick + " 472 " + mode + " :is unknown mode char to me")
#define ERR_INVITEONLYCHAN(nick, channel) (":" + nick + " 473 " + channel + " :Cannot join channel (+i)")
#define ERR_BADCHANNELKEY(nick, channel) (":" + nick + " 475 " + channel + " :Cannot join channel (+k)")
#define ERR_CHANNELISFULL(nick, channel) (":" + nick + " 471 " + channel + " :Cannot join channel (+l)")

#define RPL_NOTOPIC(client, channel) "331 " + client + " " + channel + " :No topic is set"
#define RPL_TOPIC(client, channel, topic) "332 " + client + " " + channel + " :" + topic
#define RPL_NICK(oldPrefix, newNick) ":" + oldPrefix + " NICK :" + newNick
#define RPL_JOIN(prefix, channel) ":" + prefix + " JOIN :" + channel
#define RPL_WELCOME(client) "001 " + client + " :Welcome to the Internet Relay Network ft_irc " + client
#define RPL_NAMREPLY(client, channel, namesList) "353 " + client + " = " + channel + " :" + namesList
#define RPL_ENDOFNAMES(client, channel) "366 " + client + " " + channel + " :End of /NAMES list."
#define RPL_INVITING(nickname, channel, inviter) "341 " + nickname + " " + channel + " " + inviter
#define RPL_LISTSTART(client) "321 " + client + " Channel :Users Name"
#define RPL_LIST(client, channel, users, topic) "322 " + client + " " + channel + " " + users + " :" + topic
#define RPL_LISTEND(client) "323 " + client + " :End of /LIST"
#define RPL_CHANNELMODEIS(nickname, channel, modes) "324 " + nickname + " " + channel + " " + modes
#define RPL_KICK(kicker, channel, user, comment) ":" + kicker + " KICK " + channel + " " + user + " :" + comment
#define RPL_MODE_ALREADY_ACTIVE(client, channel, mode) "MODE " + channel + " " + mode + " :Mode is already active"
#define RPL_TOPIC_ALREADY_SET(client, channel) "332 " + client + " " + channel + " :Topic is already set"

#define PART_MESSAGE(clientPrefix, channel, reason) ":" + clientPrefix + " PART " + channel + (reason.empty() ? "" : " :" + reason)

#define NOTICE_CANNOTSETLIMIT(prefix, channel) (":" + prefix + " NOTICE " + channel + " :Cannot set limit lower than current users")
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

#define NOTICE_BOT_ALREADY_PRESENT(prefix, channel) (":" + prefix + " NOTICE " + channel + " :The bot is already in this channel")
#define NOTICE_BOT_NOT_FOUND(prefix, channel) (":" + prefix + " NOTICE " + channel + " :The bot is not found")
#define NOTICE_BOT_NOSUCHCHANNEL(prefix, channel) (":" + prefix + " NOTICE " + channel + " :No such channel")
#define NOTICE_BOT_NEED_MORE_PARAM(prefix, channel) (":" + prefix + " NOTICE " + channel + " :If you want add the bot on this channel, please type /ADDBOT <channelName>")
#define NOTICE_BOT_HELP_INFO(prefix, Channel) (":" + prefix + " NOTICE " + channel + " :Type !help to see available bot commands")
#define NOTICE_BOT_HELP_HEADER(prefix, channel) (":" + prefix + " NOTICE " + channel + " :Available commands:")
#define NOTICE_BOT_HELP_TIME(prefix, channel) (":" + prefix + " NOTICE " + channel + " :<!time>:\t\tdisplay hour in the channel")
#define NOTICE_BOT_HELP_PUNCHLINE(prefix, channel) (":" + prefix + " NOTICE " + channel + " :<!punchline>:\tdisplay random punchline of 42students")
#define NOTICE_BOT_TIME(prefix, channel, time) (":" + prefix + " NOTICE " + channel + " :Current time is " + currentTime)
#define NOTICE_BOT_PUNCHLINE(prefix, channel, punchline, artist) (":" + prefix + " NOTICE " + channel + " :" + punchline + " " + artist)