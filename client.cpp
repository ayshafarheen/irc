#include "IRC.hpp"

Client::Client(int socket)
{
	nickname = "";
	username = "";
	user_type = "";
	msg = "";
	fd = socket;
	auth = 0;
}

Client::Client()
{
	nickname = "";
	username = "";
	user_type = "";
	msg = "";
	fd = -1;
	auth = 0;
}
void Client::set_msg(std::string newmsg)
{
	msg = newmsg;
}

int Client::get_fd()
{
	return fd;
}

void Client::set_auth(int auth)
{
	this->auth = auth;
}

std::string Client::get_msg()
{
	return msg;
}

Client::Client(const Client &obj)
{
	*this = obj;
}

Client & Client::operator=(const Client &obj)
{
	if(this != &obj)
	{
		nickname = obj.nickname;
		username = obj.username;
		user_type = obj.user_type;
		msg = obj.msg;
		fd = obj.fd;
		auth = obj.auth;
	}
	return *this;
}