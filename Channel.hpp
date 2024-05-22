

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
class Server;

class Channel
{
	private:
	std::string name;
	std::string topic;
	std::string				key;
	typedef std::map<std::string, Client *>::iterator ite;
	std::vector<int> clients;
	std::map<std::string, Client *> joined;
	std::map<std::string, Client *> banned;
	std::map<std::string, Client *> opers;
	bool					isInv;
	bool					hasPass;
	bool					topRestrict;
	int 					usrLim;


	

	public:
			Channel();
			Channel(std::string serv_name, Client *maker);
			~Channel();
			void addMember(Client *member);
			void setOper(Client *member);
			std::string	getServName();
			std::string getTopic();
			int 		getUsrLim();
			int 		getSize();
			std::string getKey();
			std::string sendToAll(Client &client, std::string msg, std::string cmd, bool chan);
			void kickMember(Client *member, const std::string &reason);
			bool isInChan(std::string member);
			

};

#endif