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

std::string Channel::getChanName()
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
		return ;
	}
	else if (!opers.empty())
	{
		for(ite opIt = opers.begin(); opIt != opers.end(); opIt++)
		{
			if (this->isInChan(member) == true)
			{
				return member->send_msg(ERR_USERONCHANNEL(member->get_user(), member->get_nick(), this->getChanName(), member->get_servername()));
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

bool Channel::isInvited( std::string Nick ) {
    ite inv;

    inv = invited.find(Nick);
    if (inv == invited.end())
        return false;
    return true;
}

void Channel::addMember(Client *member)
{
		if (this->isInChan(member) == true){
			std::cout <<"dfsfs" <<std::endl;
			return member->send_msg(ERR_USERONCHANNEL(member->get_user(), member->get_nick(), this->getChanName(), member->get_servername()));
		}
		if (this->isInChan(member) == false){

			joined.insert(std::pair<std::string, Client *>(member->get_nick(), member));
		}
	 sendToAll(*member, "", "JOIN", true);
}

std::string Channel::sendToAll(Client &client, std::string msg, std::string cmd, bool chan)
{
	std::string userInfo = client.get_nick() + "!" + client.get_user() + "@" + "localhost";
	std::string fullmsg = ":" + userInfo + " " + cmd;
	if (chan)
		fullmsg += " " + this->name;

	fullmsg += msg + "\r\n";
	std::cout << "here\n";

	for (ite it = joined.begin(); it != joined.end(); it++)
	{
		Client *member = it->second;
		member->send_msg(fullmsg);
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

void	Channel::addToInvite(std::string name, Client &client, Client *invitor)
{
	invited.insert(std::pair<std::string, Client*>(name, &client));
	invitor->send_msg(RPL_INVITING(user_id(invitor->get_nick(), invitor->get_user(), invitor->get_servername()), invitor->get_nick(), client.get_nick(), this->getChanName()));
}

std::string Channel::getMemberList()
{
	std::string list;
	ite itr;
	for (itr = joined.begin(); itr !=  joined.end(); itr++)
	{
		list += itr->second->get_nick() + " ";
	}
	return list;
}