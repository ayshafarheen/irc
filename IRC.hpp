#ifndef IRC_HPP
#define IRC_HPP

#include <iostream>
#include <string>
#include <exception>
#include <map>
#include <unistd.h>
#include <fstream>
#include <sys/stat.h>
#include <sstream>
#include <ctime>
#include <stdlib.h>
#include <vector>
#include <cstring>
#include <list>
#include <sys/socket.h>
#include <netdb.h>
#include <vector>
#include "numericalReplies.hpp"

class Channel
{
	std::vector<int> clients;
	std::string topic;
};

class Client
{
	std::string nickname;
	std::string username;
	// std::string user_type;
	std::string msg;
	int auth;
	int fd;

	public:
	void set_msg(std::string);
	void set_auth(int auth);
	void send_msg(std::string msg);
	std::string get_msg();
	int get_fd();
	Client(const Client &obj);
	Client &operator=(const Client &obj);
	static void create_client();
	Client();
	Client(int socket);
	int get_auth();
	std::string get_nick();
	void set_nick(std::string nick);
	std::string get_user();
	void set_user(std::string user);
	static int invalid_nick(std::string);
};

class Server
{
	private:
	sockaddr_in serverAddr;
	static int server;
	int maxfd;
	std::map<std::string, Client> clients;
	std::map<std::string, Client> auth_clients;
	static std::string password;
	void command_quit_parsing(const std::string &args, Client client);
	void command_join_parsing(const std::string &args, Client client);
	void command_kick_parsing(const std::string &args, Client client);
	void command_invite_parsing(const std::string &args, Client client);
	void command_mode_parsing(const std::string &args, Client client);
	void command_user_parsing(const std::string &args, Client client);
	void command_nick_parsing(const std::string &args, Client client);
	void command_pass_parsing(const std::string &args, Client client);
	int port;
	fd_set current_sockets;
	std::string msg;

	public:
	void set_server();
	static std::string get_pass();
	Server(char **argv);
	static int get_server();
	int accept_new_connection(int server);
	static void serv_handle(int n);
	void handle_connection(int clientsocket);
	void accept_connections();
	void check_message();
	// void command_pass_parsing(const std::string &args, Client client);
	// static void set_pass(std::string pass);
	void parse_and_execute_client_command(const std::string &clientmsg, Client client);
};

//commands
/*
KICK
INVITE
TOPIC
MODE
	 Set/remove Invite-only channel
	 Set/remove the restrictions of the TOPIC
	 Set/remove the channel key
	 Give/take channel operator privilege
	 Set/remove the user limit to channel
USER
NICK
PASS
CREATE
JOIN
PRIVMSG
*/

#endif
