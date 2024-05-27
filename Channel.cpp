#include "Channel.hpp"

Channel::Channel()
{
}

Channel::Channel(std::string serv_name, Client *maker) : name(serv_name)
{
	topic = "[NULL]";
	joined.insert(std::pair<std::string, Client *>(maker->get_nick(), maker));
	this->isInv = false;
	this->hasPass = false;
	this->topRestrict = false;
	this->usrLim = -1;
	this->inviteOnly = false;
	this->topicMode = false;
	this->passwordNeeded = false;
	maker->send_msg(RPL_JOIN(maker->get_nick(), serv_name));
}

Channel::~Channel()
{
}

bool	Channel::getInviteOnlyMode()
{
	return (this->inviteOnly);
}

bool	Channel::getTopicMode()
{
	return (this->topicMode);
}

bool	Channel::getPasswordNeeded()
{
	return (this->passwordNeeded);
}

std::string Channel::getServName()
{
	return (this->name);
}

bool 	 Channel::getHasPass()
{
	return(hasPass);
}
void Channel::setTopic(std::string top)
{
	topic = top;
}

std::string Channel::getTopic()
{
	return (this->topic);
}
int Channel::getUsrLim()
{
	return usrLim;
}

int Channel::getSize()
{
	int count = 0;
	ite memIt;
	for (memIt = joined.begin(); memIt != joined.end(); memIt++)
	{
		count++;
	}
	return count;
}
std::string Channel::getKey()
{
	return key;
}

bool	Channel::isInChan(Client* member)
{
	if(joined.find(member->get_nick()) != joined.end())
		return (true);
	else
		return (false);
}

void	Channel::setOper(Client *member)
{
	if (opers.empty()){
		member->set_oper(true);
		opers.insert(std::pair<std::string, Client *>(member->get_nick(), member));
		member->send_msg(RPL_YOUREOPER(member->get_nick()));
	}
	else if (!opers.empty())
	{
		for(ite opIt = opers.begin(); opIt != opers.end(); ++opIt)
		{
			if (opers.find(member->get_nick()) != opers.end())
			{
				member->send_msg(ERR_USERONCHANNEL(member->get_user(), member->get_nick(), this->getServName(), member->get_servername()));
			}
			else
			{
				opers.insert(std::pair<std::string, Client *>(member->get_nick(), member));
				member->send_msg(RPL_YOUREOPER(member->get_nick()));
				member->set_oper(true);
			}
		}
	}
}


void Channel::addMember(Client *member)
{
	ite iter;
	for (iter = joined.begin(); iter != joined.end(); iter++)
	{
		if (joined.find(member->get_nick()) != joined.end())
		{
			member->send_msg(ERR_USERONCHANNEL(member->get_user(), member->get_nick(), this->getServName(), member->get_servername()));
		}
		else{
			joined.insert(std::pair<std::string, Client *>(member->get_nick(), member));}
	}
	sendToAll(*member, "", "JOIN", true);
}

std::string Channel::sendToAll(Client &client, std::string msg, std::string cmd, bool chan)
{
	std::string userInfo = client.get_nick() + "!" + client.get_user() + "@" + "localhost";
	std::string fullmsg = ":" + userInfo + " " + cmd;
	if (chan)
		fullmsg += " " + this->name;
	// if (msg.at(0)!= ':')
	// 	msg = ":" + msg;
	fullmsg += msg + "\r\n";
	std::cout << "here\n";
	// std::map<std::string, Client *> sent;
	for (ite it = joined.begin(); it != joined.end(); it++)
	{
		Client *member = it->second;
		// if (member->get_nick() != client.get_nick() && sent.find(member->get_nick()) == sent.end()){
		member->send_msg(fullmsg);
		// sent[member->get_nick()] = member;
		// }
	}

	return (fullmsg);
}

void Channel::kickMember(Client *member, const std::string &reason)
{
	ite iter = joined.find(member->get_nick());
	if (iter != joined.end())
	{
		// Notify the member that they have been kicked
		member->send_msg("You have been kicked from the channel. Reason: " + reason);
		// Notify all other members in the channel about the kick
		sendToAll(*member, reason, "KICK", true);
		// Remove the member from the channel
		joined.erase(iter);
	}
	else
	{
		// Member is not in the channel, send an error message
		member->send_msg("Error: You are not a member of this channel.");
	}
}

void Channel::memberQuit(Client *member, const std::string &reason)
{
	ite iter = joined.find(member->get_nick());

	if (iter != joined.end())
	{
		sendToAll(*member, reason, "QUIT", true);
		joined.erase(iter);
	}
}

void	Channel::addToInvite(std::string name, Client *client)
{
	
}