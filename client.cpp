#include "IRC.hpp"

Client::Client(int socket)
{
	nickname = "";
	username = "";
	msg = "";
	fd = socket;
	auth = 0;
}

Client::Client()
{
	nickname = "";
	username = "";
	msg = "";
	fd = -1;
	auth = 0;
}

int Client::invalid_nick(std::string nick)
{
	for(unsigned long i=0 ; i < nick.length(); i++)
	{
		if(isalnum(nick[i]) || nick[i] == '}' || nick[i] == '{' || nick[i] == '[' || nick[i] ==']' || nick[i] =='|' || nick[i] == '\\')
			continue;
		else
			return 1;
	}
	return 0;
}

void Client::set_msg(std::string newmsg)
{
	msg = newmsg;
}

int Client::get_fd() const
{
	return fd;
}

int Client::get_auth() const
{
	return auth;
}


void Client::send_msg(std::string msg)
{
	const char* message = msg.c_str();
	send(fd, message, strlen(message), 0);

}

void Client::set_auth(int auth)
{
	this->auth = auth;
}

std::string Client::get_msg() const
{
	return msg;
}

std::string Client::get_nick() const
{
	return nickname;
}

void Client::set_nick(std::string nick)
{
	nickname = nick;
}

void Client::set_user(std::string user)
{
	username = user;
}

std::string Client::get_user() const
{
	return username;
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
		msg = obj.msg;
		fd = obj.fd;
		auth = obj.auth;
	}
	return *this;
}

std::ostream& operator<<(std::ostream& out, const Client &client)
{
	return out << client.get_auth() << " " << client.get_nick() << " " << client.get_user() << "\n";
}