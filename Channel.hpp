

#ifndef CHANNEL_H
# define CHANNEL_H

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
#include "IRC.hpp"
#include <stdbool.h>

class Client;

class Channel
{
	private:
	std::string name;
	std::string topic;
	typedef std::map<std::string, Client *>::iterator ite;
	std::vector<int> clients;
	std::string topic;
	std::map<std::string, Client *> joined;
	std::map<std::string, Client *> banned;
	bool					isOper;
	bool					isInv;
	bool					hasPass;
	int 					usrLim;



	

	public:
			Channel(std::string serv_name, Client *maker);
			~Channel();
			void addMember();
			std::string	getServName();
			std::string getTopic();
			

};

#endif