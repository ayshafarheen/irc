#include "IRC.hpp"

Client::Client(int socket)
{
	char _hostname[1024];
	nickname = "";
	username = "";
	msg = "";
	fd = socket;
	auth = 0;

	gethostname(_hostname, 1024);
	hostname = (std::string)_hostname;
}

Client::Client()
{
	char _hostname[1024];
	gethostname(_hostname, 1024);
	hostname = (std::string)_hostname;
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
		if(isalnum(nick[i]) || nick[i] == '}' || nick[i] == '_' || nick[i] == '{' || nick[i] == '[' || nick[i] ==']' || nick[i] =='|' || nick[i] == '\\')
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

void Client::set_hostname(std::string newmsg)
{
	hostname = newmsg;
}

void Client::set_servername(std::string newmsg)
{
	servername = newmsg;
}

void Client::set_realname(std::string newmsg)
{
	realname = newmsg;
}

int Client::get_fd() const
{
	// std::cout << fd << std::endl;
	return fd;
}

int Client::get_auth() const
{
	return auth;
}


void Client::send_msg(std::string msg)
{
	std::cout << "Sending " << msg << "\n";
	const char* message = msg.c_str();
	if(send(fd, message, strlen(message), 0) == -1)
		std::cerr<< "Sending error!\n";

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

std::string Client::get_hostname() const
{
	return hostname;
}

std::string Client::get_servername() const
{
	return servername;
}

std::string Client::get_realname() const
{
	return realname;
}

Client::Client(const Client &obj)
{
	*this = obj;
}

Client & Client::operator=(const Client &obj)
{
	if(this != &obj)
	{
		char _hostname[1024];
		gethostname(_hostname, 1024);
		hostname = (std::string)_hostname;
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