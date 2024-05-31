#include "Channel.hpp"

Channel::Channel()
{
}

Channel::Channel(std::string serv_name, Client *maker) : name(serv_name)
{
	topic = "";
	joined.insert(std::pair<std::string, Client *>(maker->get_nick(), maker));
	this->hasPass = false;
	this->topRestrict = false;
	this->usrLim = -1;
	this->inviteOnly = false;
	this->topicMode = false;
	setOper(maker);
	welcome(maker);
}

Channel::~Channel()
{
}

bool Channel::getInviteOnlyMode()
{
	return (this->inviteOnly);
}

bool Channel::getTopicMode()
{
	return (this->topicMode);
}

std::string Channel::getChanName()
{
	return (this->name);
}

bool Channel::getHasPass()
{
	return (hasPass);
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

bool Channel::isInChan(Client *member)
{
	if (joined.find(member->get_nick()) != joined.end())
		return (true);
	else
		return (false);
}

void Channel::setOper(Client *member)
{
	if (opers.empty())
	{
		member->set_oper(true);
		opers.insert(std::pair<std::string, Client *>(member->get_nick(), member));
		member->send_msg(RPL_YOUREOPER(member->get_nick()));
		return;
	}
	else if (!opers.empty())
	{
		for (ite opIt = opers.begin(); opIt != opers.end(); opIt++)
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
bool Channel::isOper(Client *member)
{
	ite inv;

	inv = opers.find(member->get_nick());
	if (inv == opers.end() && !member->getOper())
		return false;
	return true;
}
bool Channel::isInvited(Client *member)
{
	ite inv;

	inv = invited.find(member->get_nick());
	if (inv == invited.end())
		return false;
	return true;
}

std::string Channel::getMemberList()
{
	std::string list;
	ite itr;
	for (itr = joined.begin(); itr != joined.end(); itr++)
	{
		if (isOper(itr->second))
			list += '@' + itr->second->get_nick() + " ";
		else
			list += itr->second->get_nick() + " ";
	}
	return list;
}

bool Channel::validEntrance(Client *member, std::string key)
{
	if (getHasPass() && (key.compare(getKey()) != 0))
	{
		member->send_msg(ERR_BADCHANNELKEY(member->get_nick(), name));
		return false;
	}
	if (getInviteOnlyMode() && (!isInvited(member)))
	{
		member->send_msg(ERR_INVITEONLYCHAN(member->get_nick(), name, member->get_servername()));
		return false;
	}
	if (usrLim > 0 && (this->getSize() >= usrLim))
	{
		member->send_msg(ERR_CHANNELISFULL(member->get_nick(),name));
		return false;
	}
	return true;
}

void Channel::addMember(Client *member, std::string key)
{
		if (this->getSize() > 1 && this->isInChan(member) == true){
			return member->send_msg(ERR_USERONCHANNEL(member->get_user(), member->get_nick(), this->getChanName(), member->get_servername()));
		}
		if (this->isInChan(member) == false && this->validEntrance(member,key))
		{
				joined.insert(std::pair<std::string, Client *>(member->get_nick(), member));
	 			welcome(member);
		}
		if (this->isInvited(member) == true)
		{
			invited.erase(member->get_nick());
		}
}

std::string Channel::sendToAll(Client &client, std::string msg, std::string cmd, bool chan)
{
	(void)chan;
	for (ite it = joined.begin(); it != joined.end(); it++)
	{
		Client *member = it->second;
		if (cmd == "PRIVMSG")
		{
			if (member->get_fd() != client.get_fd())
				member->send_msg(msg);
		}
		else
			member->send_msg(msg);
	}
	return (msg);
}

void Channel::kickMember(Client *member, const std::string &reason, Client *target)
{
	ite iter = joined.find(target->get_nick());
	if (iter != joined.end())
	{
		// Notify the member that they have been kicked
		target->send_msg(RPL_KICK(member->get_nick(), getChanName(), target->get_nick(), reason));
		// Notify all other members in the channel about the kick
		sendToAll(*member, RPL_KICK(member->get_nick(), getChanName(), target->get_nick(), reason), "KICK", true);
		member->send_msg(RPL_KICK(member->get_nick(), getChanName(), target->get_nick(), reason));
		if (opers.find(member->get_nick()) != opers.end())
			opers.erase(member->get_nick());
		// Remove the member from the channel
		joined.erase(iter);
	}
	else
	{
		// Member is not in the channel, send an error message
		member->send_msg(ERR_USERNOTINCHANNEL(member->get_nick(), target->get_nick(), getChanName()));
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

void Channel::addToInvite(std::string name, Client *client, Client *invitor)
{
	invited.insert(std::pair<std::string, Client *>(name, client));
	invitor->send_msg(RPL_INVITING(invitor->get_id(), invitor->get_nick(), client->get_nick(), this->getChanName()));
}

void Channel::welcome(Client *member)
{
	std::string msg = "\n       WELCOME TO CHANNEL " + name + " !!!\n\n";
	this->sendToAll(*member, RPL_JOIN(member->get_id(), name), "JOIN", true);
	// member->send_msg(MODE_CHANNELMSG(name, ))
	member->send_msg(msg);
	if (!this->getTopic().empty())
		member->send_msg(RPL_TOPIC(member->get_nick(), name, this->getTopic(), member->get_servername()));
	member->send_msg(RPL_NAMREPLY(member->get_nick(), '@', name, this->getMemberList()));
	member->send_msg(RPL_ENDOFNAMES(member->get_nick(), name, member->get_servername()));
	;
}

void Channel::change_in_all(std::string oldnick, Client &client, std::string cmd)
{
	// std::cout << "pls work " << joined.find(oldnick)->second->get_nick() << std::endl;
	if (cmd == "NICK")
	{
		if (joined.find(oldnick) != joined.end())
		{
			joined[client.get_nick()] = &client;
			joined.erase(oldnick);
		}
		if (invited.find(oldnick) != invited.end())
		{
			invited[client.get_nick()] = &client;
			invited.erase(oldnick);
		}
		if (opers.find(oldnick) != opers.end())
		{
			opers[client.get_nick()] = &client;
			opers.erase(oldnick);
		}
	}
	else if (cmd == "LEAVE")
	{
		if (joined.find(client.get_nick()) != joined.end())
			joined.erase(client.get_nick());
		if (invited.find(client.get_nick()) != invited.end())
			invited.erase(client.get_nick());
		if (opers.find(client.get_nick()) != opers.end())
			opers.erase(client.get_nick());
	}
}