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

#ifndef CHANNEL_H
# define CHANNEL_H

class Channel : public Server
{
	private:
	typedef std::map<std::string, Client *>::iterator ite;
	std::vector<int> clients;
	std::string topic;
	std::map<std::string, Client *> joined;
	std::map<std::string, Client *> banned;
	bool					isOper;


	

	public:
			Channel(std::string serv_name, Client *maker);
			~Channel();
};

#endif