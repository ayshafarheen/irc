#include "Channel.hpp"

Channel::Channel(std::string serv_name, Client *maker) : name(serv_name)
{
	topic = "[NULL]";
	// std::string val = std::to_string(maker->get_fd());
	joined.insert(std::pair<std::string, Client *>( std::to_string(maker->get_fd()), maker));
	this->isOper = false;
	this->isInv = false;
	this->usrLim = -1;
	std::cout << CYAN "hi channel: " << serv_name << "has been created" << std::endl;
}

Channel::~Channel(){

}

std::string Channel::getServName()
{
	return (this->name);
}

std::string Channel::getTopic()
{
	return (this->topic);
}