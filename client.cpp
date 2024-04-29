#include "IRC.hpp"

Client::Client(int socket)
{
	nickname = "";
	username = "";
	user_type = "";
	msg = "";
	fd = socket;
}

Client::Client()
{
	nickname = "";
	username = "";
	user_type = "";
	msg = "";
	fd = -1;
}
void Client::set_msg(std::string newmsg)
{
	msg = newmsg;
}

int Client::get_fd()
{
	return fd;
}

std::string Client::get_msg()
{
	return msg;
}

Client::Client(Client &obj)
{
	*this = obj;
}


Client & Client::operator=(Client &obj)
{
	if(this != &obj)
	{
		nickname = obj.nickname;
		username = obj.username;
		user_type = obj.user_type;
		msg = obj.msg;
		fd = obj.fd;
	}
	return *this;
}
