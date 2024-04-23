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

	public:
	static void create_client();
};

class Server
{
	private:
	sockaddr_in serverAddr;
	int server;
	std::vector<int> clients;
	std::string password;
	int port;

	public:
	void set_server();
	Server(char **argv);
	int get_server();
	void serv_handle(int n);
	void accept_connections();
};


#endif
