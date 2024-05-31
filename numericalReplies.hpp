#ifndef NUMERICAL_REPLIES_HPP
#define NUMERICAL_REPLIES_HPP

#include <iostream>
#include <string>

void	sendServerRpl(int const client_fd, std::string client_buffer);

# define user_id(nickname, username, server) (":" + nickname + "!" + username + "@" + server+ "")

# define RPL_WELCOME(user_id, nickname, server) (":" + server+ " 001 " + nickname + " :Welcome to the Internet Relay Network " + user_id + "\r\n")
# define RPL_YOURHOST(client, servername, version, server) (":" + server + " 002 " + client + " :Your host is " + servername + " (" + server+ "), running version " + version + "\r\n")
# define RPL_CREATED(client, datetime, server) (":" + server+ " 003 " + client + " :This server was created " + datetime + "\r\n")
# define RPL_MYINFO(client, servername, version, user_modes, chan_modes, chan_param_modes) (":" + servername+ " 004 " + client + " " + servername + " " + version + " " + user_modes + " " + chan_modes + " " + chan_param_modes + "\r\n")
# define RPL_ISUPPORT(client, tokens, server) (":" + server+ " 005 " + client + " " + tokens " :are supported by this server\r\n")

# define ERR_UNKNOWNCOMMAND(client, command, server) (":" + server+ " 421 " + client + " " + command + " :Unknown command\r\n")

// INVITE
# define ERR_NEEDMOREPARAMS(client, command, server) (":" + server+ " 461 " + client + " " + command + " :Not enough parameters.\r\n")
# define ERR_INVITEONLYCHAN(client, channel, server) (":" + server+ " 473 " + client + " " + channel + " :Cannot join channel (+i)")
# define ERR_NOSUCHCHANNEL(client, channel, server) (":" + server+ " 403 " + client + " " + channel + " :No such channel\r\n")
# define ERR_NOTONCHANNEL(client, channel, server) (":" + server+ " 442 " + client +  " "+ channel + " :The user is not on this channel.\r\n")
# define ERR_USERONCHANNEL(client, nick, channel, server) (":" + server+ " 443 " + client + " " + nick + " " + channel + " :Is already on channel\r\n")
# define RPL_INVITING(user_id, client, nick, channel) (user_id + " 341 " + client + " " + nick + " " + channel + "\r\n")
# define RPL_INVITE(user_id, invited, channel) (user_id + " INVITE " + invited + " " + channel + "\r\n")

// JOIN
# define RPL_JOIN(user_id, channel) (user_id + " JOIN :" +  channel + "\r\n")
# define ERR_BANNEDFROMCHAN(client, channel) ("474 " + client + " " + channel + " :Cannot join channel (+b)\r\n")
# define ERR_BADCHANNELKEY(client, channel) ("475 " + client + " " + channel + " :Cannot join channel (+k)\r\n")

// KICK
# define ERR_USERNOTINCHANNEL(client, nickname, channel) ("441 " + client + " " + nickname + " " + channel + " :They aren't on that channel\r\n")
// # define ERR_CHANOPRIVSNEEDED(client, channel) ("482 " + client + " #" +  channel + " :You're not channel operator\r\n")
# define RPL_KICK(user_id, channel, kicked, reason) (user_id + " KICK " + channel + " " + kicked + " " + reason + "\r\n")

// KILL
# define ERR_NOPRIVILEGES(client) ("481 " + client + " :Permission Denied- You're not an IRC operator\r\n")
# define RPL_KILL(user_id, killed, comment) (user_id + " KILL " + killed + " " + comment + "\r\n")

// MODE
/* user mode */
#define MODE_USERMSG(client, mode) (":" + client + " MODE " + client + " :" + mode + "\r\n")
#define ERR_UMODEUNKNOWNFLAG(client, server) (":" + server+ " 501 " + client + " :Unknown MODE flag\r\n")
#define ERR_USERSDONTMATCH(client) ("502 " + client + " :Cant change mode for other users\r\n")
#define RPL_UMODEIS(client, mode, server) (":" + server+ " 221 " + client + " " + mode + "\r\n")
/* channel mode */
#define ERR_BADCHANMASK(nick,server) (":" + server + " 476 * " + nick + " :Bad Channel Mask\r\n")
#define MODE_CHANNELMSG(channel, mode, server) (":" + server+ " MODE " + channel + " " + mode + "\r\n")
#define MODE_CHANNELMSGWITHPARAM(channel, mode, param, server) (":" + server+ " MODE " + channel + " " + mode + " " + param + "\r\n")
#define RPL_CHANNELMODEIS(client, channel, mode, server) (":" + server+ " 324 " + client + " " + channel + " " + mode + "\r\n")
#define RPL_CHANNELMODEISWITHKEY(client, channel, mode, password, server) (":" + server+ " 324 " + client + " #" + channel + " " + mode + " " + password + "\r\n")
#define ERR_CANNOTSENDTOCHAN(client, channel) ("404 " + client + " " + channel + " :Cannot send to channel\r\n")
#define ERR_CHANNELISFULL(client, channel) ("471 " + client + " " + channel + " :Cannot join channel (+l)\r\n")
#define ERR_CHANOPRIVSNEEDED(client, channel, server) (":" + server+ " 482 " + client + " " + channel + " :You're not channel operator\r\n")
#define ERR_INVALIDMODEPARAM(client, channel, mode, password) ("696 " + client + " " + channel + " " + mode + " " + password + " : password must only contained alphabetic character\r\n")
// RPL_ERR a broadcoast quand user pas +v ou operator veut parler
      // dans notre cas c'était tiff (client) qui voulait send a message
      // :lair.nl.eu.dal.net 404 tiff #pop :Cannot send to channel
#define RPL_ADDVOICE(nickname, username, channel, mode, param, server) (":" + nickname + "!" + username + "@" + server+ " MODE #" + channel + " " + mode + " " + param + "\r\n")

// MOTD
#define ERR_NOSUCHSERVER(client, servername) (":" + server+ " 402 " + client + " " + servername + " :No such server\r\n")
// #define ERR_NOMOTD(client) (":" + server+ " 372 " + client + " :" + motd_line + "\r\n")
#define ERR_NOMOTD(client) (": 422 " + client + " :MOTD File is missing\r\n")
#define RPL_MOTDSTART(client, servername) (":" + servername+ " 375 " + client + " :- " + servername + " Message of the day - \r\n")
#define RPL_MOTD(client, motd_line, server) (":" + server+ " 372 " + client + " :" + motd_line + "\r\n")
#define RPL_ENDOFMOTD(client, server) (":" + server+ " 376 " + client + " :End of /MOTD command.\r\n")


// NAMES
# define RPL_NAMREPLY(client, symbol, channel, list_of_nicks) (":localhost 353 " + client + " " + symbol + " " + channel + " :" + list_of_nicks + "\r\n")
# define RPL_ENDOFNAMES(client, channel, server) (":" + server+ " 366 " + client +  " " + channel + " :End of /NAMES list.\r\n")

// NICK
# define ERR_NONICKNAMEGIVEN(client, server) (":" + server+ " 431 " + client + " :There is no nickname.\r\n")
# define ERR_ERRONEUSNICKNAME(client, nickname, server) (":" + server+ " 432 " + client + " " + nickname + " :Erroneus nickname\r\n")
# define ERR_NICKNAMEINUSE(client, nickname, server) (":" + server+ " 433 " + client + " " + nickname + " :Nickname is already in use.\r\n")
# define RPL_NICK(oclient, uclient, client, server) (":" + oclient + "!" + uclient + "@" + server+ " NICK " +  client + "\r\n")

// USER
# define ERR_ALREADYREGISTERED(client, server) (":" + server+ " 462 " + client + " :You may not reregister.\r\n")

// NOTICE
# define RPL_NOTICE(nick, username, target, message, server) (":" + nick + "!" + username + "@" + server+ " NOTICE " + target + " " + message + "\r\n")

// OPER
# define ERR_NOOPERHOST(client) ("491 " + client + " :No O-lines for your host\r\n")
# define RPL_YOUREOPER(client) ("381 " + client + " :You are now an IRC operator\r\n")

// PART
# define RPL_PART(user_id, channel, reason) (user_id + " PART #" + channel + " " + (reason.empty() ? "." : reason ) + "\r\n")

// PASS
# define ERR_PASSWDMISMATCH(client, server) (":" + server+ " 464 " + client + " :Password incorrect.\r\n")

// PING
# define RPL_PONG(server, token) ("PONG " + server + " " + token  + "\r\n")

// QUIT
# define RPL_QUIT(user_id, reason) (user_id + " QUIT :Quit: " + reason + "\r\n")
# define RPL_ERROR(user_id, reason) (user_id + " ERROR :" + reason + "\r\n")

// PRIVMSG
# define ERR_NOSUCHNICK(client, target) ("401 " + client + " " + target + " :No such nick/channel\r\n")
# define ERR_NORECIPIENT(client) ("411 " + client + " :No recipient given PRIVMSG\r\n")
# define ERR_NOTEXTTOSEND(client) ("412 " + client + " :No text to send\r\n")
# define RPL_PRIVMSG(nick, username, target, message, server) (":" + nick + "!" + username + "@" + server+ " PRIVMSG " + target + " " + message + "\r\n")

// TOPIC
# define RPL_TOPIC(client, channel, topic, server) (":" + server+ " 332 " + client + " "+ channel + " " + topic + "\r\n")
# define RPL_NOTOPIC(client, channel, server) (":" + server+ " 331 " + client + " #" + channel + " :No topic is set\r\n")
# define RPL_NOTOPIC(client, channel, server) (":" + server+ " 331 " + client + " #" + channel + " :No topic is set\r\n")
# define RPL_TOPIC_CHANGE(nick, username, channel, topic, server) (":" + nick + "!" + username + "@" + server+ " TOPIC " + channel + " " + topic + "\r\n")



#endif
