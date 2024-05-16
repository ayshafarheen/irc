#include "Channel.hpp"

Channel::Channel()
{

}

Channel::Channel(std::string serv_name, Client *maker) : name(serv_name)
{
	topic = "[NULL]";
	joined.insert(std::pair<std::string, Client *>(maker->get_nick(), maker));
	this->isOper = false;
	this->isInv = false;
	this->hasPass = false;
	this->usrLim = -1;
	maker->send_msg(RPL_JOIN(maker->get_nick(), serv_name));
}

Channel::~Channel()
{

}

std::string Channel::getServName()
{
	return (this->name);
}

std::string Channel::getTopic()
{
	return (this->topic);
}

void	Channel::addMember(Client *member)
{
	ite iter;
	for (iter = joined.begin(); iter != joined.end(); iter++)
	{
		if (joined.find(member->get_nick()) != joined.end())
		{
			member->send_msg(ERR_USERONCHANNEL(member->get_user(),member->get_nick(), this->getServName()));
		}
		else
			joined.insert(std::pair<std::string, Client *>(member->get_nick(), member));
	}
}