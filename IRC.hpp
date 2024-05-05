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

class Channel
{
	std::vector<int> clients;
	std::string topic;
};

class Client
{
	std::string nickname;
	std::string username;
	std::string user_type;
	std::string msg;
	int fd;

	public:
	void set_msg(std::string);
	std::string get_msg();
	int get_fd();
	Client(const Client &obj);
	Client &operator=(const Client &obj);
	static void create_client();
	Client();
	Client(int socket);
};

class Server
{
	private:
	sockaddr_in serverAddr;
	int server;
	int maxfd;
	std::map<std::string, Client> clients;
	std::string password;
	int port;
	fd_set current_sockets;
	std::string msg;

	public:
	void set_server();
	Server(char **argv);
	int get_server();
	int accept_new_connection(int server);
	static void serv_handle(int n);
	void handle_connection(int clientsocket);
	void accept_connections();
};

void check_message();
void parse_and_execute_client_command(const std::string &clientmsg);

#endif
