

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
	std::map<std::string, Client *>	invited;
	std::map<std::string, Client *> opers;
	bool					hasPass;
	bool					topRestrict;
	int 					usrLim;
	bool					inviteOnly;
	bool					topicMode;

	public:
public:
			Channel();
			Channel(std::string serv_name, Client *maker);
			~Channel();
			void addMember(Client *member, std::string key);
			void setOper(Client *member);
			std::string	getChanName();
			std::string getTopic();
			void setTopic(std::string topic);
			int 		getUsrLim();
			int 		getSize();
			std::string getKey();
			bool getHasPass();
			bool isOper(Client *member);
			std::string sendToAll(Client &client, std::string msg, std::string cmd, bool chan);
			void kickMember(Client *member, const std::string &reason, Client *target);
			bool isInChan(Client *member);
			void memberQuit(Client *member, const std::string &reason);
			void setInviteOnlyMode(Client &client, bool enable);
			void setPasswordNeededTrue(Client &client, std::string &password);
			void setPasswordNeededFalse(Client &client);
			void setPrivilageMode(Client *member, bool enable);
			void setTopicMode(Client &client, bool enable);
			void setUserLimit(Client &client, int limit);
			bool getInviteOnlyMode();
			bool getTopicMode();
			void	addToInvite(std::string name, Client *client, Client *invitor);
			bool isInvited(Client *member);
			std::string getMemberList();
			void	welcome(Client *member);
			void change_in_all(std::string oldnick, Client &client, std::string cmd);
			bool validEntrance(Client *member, std::string key);
			void erase();
};
#endif