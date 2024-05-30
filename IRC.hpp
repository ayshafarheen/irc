#ifndef IRC_HPP
#define IRC_HPP

#include <iostream>
#include <string>
#include <exception>
#include <map>
#include <unistd.h>
#include <fstream>
#include <csignal>
#include <sys/stat.h>
#include <sstream>
#include <ctime>
#include <fcntl.h>
#include <stdlib.h>
#include <vector>
#include <cstring>
#include <list>
#include <sys/socket.h>
#include <netdb.h>
#include <vector>
#include "numericalReplies.hpp"
#include "Channel.hpp"
#include <ios>
#include "Commands.hpp"


class Client
{
	std::string nickname;
	std::string username;
	// std::string user_type;
	std::string msg;
	std::string hostname;
	std::string servername;
	std::string realname;
	std::string id;
	std::vector<std::string> msgs;
	int auth;
	int fd;
	bool isOper;

	public:
	void	set_oper(bool oper);
	void set_msg(std::string);
	void set_auth(int auth);
	void send_msg(std::string msg);
	std::string get_msg() const;
	int get_fd() const;
	bool getOper();
	Client(const Client &obj);
	Client &operator=(const Client &obj);
	static void create_client();
	Client();
	Client(int socket);
	int get_auth() const;
	std::string get_nick() const;
	void set_hostname(std::string newmsg);
	void set_servername(std::string newmsg);
	void receive(int socket, fd_set &current_sockets, std::map<std::string, Client> & clients, std::map<std::string, Client> & auth_clients);
	void set_id(std::string newmsg);
	void set_realname(std::string newmsg);
	std::string get_hostname() const;
	std::string get_id() const;
	std::string get_servername() const;
	std::string get_realname() const;
	void set_nick(std::string nick);
	std::string get_user() const;
	void set_user(std::string user);
	static int invalid_nick(std::string);
};


std::ostream& operator<<(std::ostream& out, const Client &client);

class Channel;
class Server
{
	private:
	sockaddr_in serverAddr;
	static int server;
	int maxfd;
	std::map<std::string, Client> clients;
	std::map<std::string, Client> auth_clients;
	std::map<std::string, Channel> channels;
	static std::string password;
	typedef std::map<std::string, Channel>::iterator itChan;
	typedef std::map<std::string, Client>::iterator itCli;
	void command_quit_parsing(const std::string &args, Client &client);
	void command_join_parsing(const std::string &args, Client &client);
	void command_kick_parsing(const std::string &args, Client &client);
	void command_invite_parsing(const std::string &args, Client &client);
	void signal_handler(int signal_num);
	void command_ping_parsing(const std::string &args, Client &client);
	void command_mode_parsing(const std::string &args, Client &client);
	void command_cap_parsing(const std::string &args, Client &client);
	void command_priv_parsing(const std::string &args, Client &client);
	void command_user_parsing(const std::string &args, Client &client);
	void command_nick_parsing(const std::string &args, Client &client);
	void command_topic_parsing(const std::string &args, Client &client);
	void command_pass_parsing(const std::string &args, Client &client);
	void command_part_parsing(const std::string &args, Client &client);
	void command_motd_parsing(const std::string &args, Client &client);

	void authenticate(Client &client);
	int port;
	fd_set current_sockets;
	std::string msg;

	public:
	void set_server();
	std::map<std::string, Client> &get_client();
	void clear_all();
	static std::string get_pass();
	static std::string to_string(int b);
	Server(char **argv);
	static int get_server();
	int accept_new_connection(int server);
	static void serv_handle(int n);
	void handle_connection(int clientsocket);
	void accept_connections();
	void check_message();
	Channel getChannel(std::string chan_name);
	// void command_pass_parsing(const std::string &args, Client client);
	// static void set_pass(std::string pass);
	void parse_and_execute_client_command(const std::string &clientmsg, Client &client);
	void commandQuit(Client *member, const std::string reason);
};
// std::vector<std::string> ft_split(coknst std::string &str, char delimiter);

//commands
/*
KICK
INVITE
TOPIC;l
	 Set/remove the user limit to channel
USER
NICK
PASS
CREATE
JOIN
PRIVMSG
*/

#endif
