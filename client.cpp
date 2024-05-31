#include "IRC.hpp"

Client::Client(int socket)
{
	char _hostname[1024];
	nickname = "";
	username = "";
	msg = "";
	fd = socket;
	auth = 0;
	isOper = false;

	gethostname(_hostname, 1024);
	hostname = (std::string)_hostname;
}
void Client::receive(int clientSocket, fd_set &current_sockets, std::map<std::string, Client> & clients, std::map<std::string, Client> & auth_clients, std::map<std::string, Channel> & channels)
{
	int fail = 0;
	char buffer[1024] = {0};
	int count = 0;
	top:
	while ((count = recv(clientSocket, &buffer[fail], sizeof(buffer) - fail, 0)) > 0)
		fail += count;
	if (fail == -1)
		std::cerr << "Error receiving message: " << strerror(errno) << "\n";
	else if (count == 0)
	{
		std::cout << "Connection closed! " << std::endl;
		FD_CLR(clientSocket, &current_sockets);
		if(auth_clients.find(nickname) != auth_clients.end() && (auth_clients.find(nickname)->second.get_fd() == clientSocket))
		{
			for (std::map<std::string,Channel>::iterator i = channels.begin(); i != channels.end(); ++i)
			{
				i->second.change_in_all("",auth_clients.find(nickname)->second,"LEAVE");
			}
			auth_clients.erase(nickname);
		}
		clients.erase(Server::to_string(clientSocket));
		close(clientSocket);
	}
	else if(strchr(buffer,'\n'))
		msg = std::string(buffer);
	else
		goto top;
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
	isOper = false;
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

void	Client::set_oper(bool oper)
{
	isOper = oper;
}
void Client::set_msg(std::string newmsg)
{
	msg = newmsg;
}

void Client::set_id(std::string newmsg)
{
	id = newmsg;
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

bool Client::getOper()
{
	return isOper;
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
	std::cout << "Sending " << msg << " to " << fd << std::endl;
	const char* message = msg.c_str();
	ssize_t messageLength = strlen(message);
	ssize_t bytesSent = send(fd, message, messageLength, 0);
	if (bytesSent == -1)
		std::cerr << "Sending error: " << strerror(errno) << "\n";
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

std::string Client::get_id() const
{
	return id;
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