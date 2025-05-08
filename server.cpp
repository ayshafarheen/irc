#include "IRC.hpp"

std::string Server::password = "";
int Server::server = 0;

void Server::clear_all()
{
	for (std::map<std::string, Channel>::iterator i = channels.begin(); i != channels.end(); ++i)
	{
		i->second.erase();
	}
	channels.clear();
	auth_clients.clear();
	clients.clear();
}

void Server::serv_handle(int n)
{
	if (n == 1)
		std::cerr << "Server error\n";
	if (n == 2)
		std::cerr << "Invalid port\n";
	if (n == 3)
		std::cerr << "Binding error\n";
	if (n == 4)
		std::cerr << "Queue size exceeded\n";
	if (n == 5)
		std::cerr << "Invalid client socket\n";
	if (n == 6)
		std::cerr << "receiving error\n";
	if (n == 7)
		std::cerr << "Select error\n";
	if (n == 8)
		std::cerr << "Invalid client socket\n";
	if (n == 9)
		std::cerr << "Incorrect password\n";
}

std::map<std::string, Client> &Server::get_client()
{
	return clients;
}

std::string Server::to_string(int b)
{
	std::stringstream ss;
	ss.str("");
	ss.clear();
	ss << b;
	std::string str2 = ss.str();
	return str2;
}

void Server::set_server()
{
	int fail;
	try
	{
		server = socket(AF_INET, SOCK_STREAM, 0);
		maxfd = server;
		if (server == -1)
			throw(1);
		this->serverAddr.sin_family = AF_INET;		   // IPV4
		this->serverAddr.sin_port = htons(port);	   // port number host to network
		this->serverAddr.sin_addr.s_addr = INADDR_ANY; // dont bind to particular ip but listen for all available IPs
		int opt = 1;
		if (setsockopt(server, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)))
		{
			std::cerr << "setsockopt(SO_REUSEADDR) failed\n";
			close(server);
			exit(0);
		}
		fail = bind(server, (const sockaddr *)&(this->serverAddr), sizeof(this->serverAddr));
		if (fail == -1)
			throw(3);
		fail = listen(server, 5);
		if (fail == -1)
			throw(4);
		accept_connections();
	}
	catch (int n)
	{
		serv_handle(n);
	}
}

void Server::authenticate(Client &client)
{
	if (client.get_user() != "" && client.get_nick() != "" && client.get_auth())
	{
		auth_clients[client.get_nick()] = clients[to_string(client.get_fd())];
		char _hostname[1024];
		std::string hostname;
		gethostname(_hostname, 1024);
		hostname = (std::string)_hostname;
		time_t now = time(0);
		char *date_time = ctime(&now);
		client.set_id(client.get_nick() + "@" + hostname);
		client.send_msg(RPL_WELCOME(user_id(client.get_nick(), client.get_user(), client.get_servername()), client.get_nick(), client.get_servername()));
		client.send_msg(RPL_YOURHOST(client.get_nick(), hostname, "1.0", client.get_servername()));
		client.send_msg(RPL_CREATED(client.get_nick(), date_time, client.get_servername()));
		command_motd_parsing("", client);
		for (std::map<std::string, Client>::iterator i = auth_clients.begin(); i != auth_clients.end(); ++i)
		{
			std::cout << ((*i).second).get_nick() << std::endl;
		}
	}
}



void Server::handle_connection(int clientSocket)
{
	Client &client = clients[to_string(clientSocket)];
	client.receive(clientSocket, current_sockets, clients, auth_clients, channels);
	if (clients.find(Server::to_string(clientSocket)) != clients.end())
		parse_and_execute_client_command(client.get_msg(), client);
}

int Server::accept_new_connection(int server)
{
	int clientSocket = accept(server, NULL, NULL);
	Client client1(clientSocket);
	if (clientSocket == -1)
		return -1;
	fcntl(clientSocket, F_SETFL, O_NONBLOCK);
	std::string val = to_string(clientSocket);
	clients[val] = client1;
	std::cout << "Accepted new connection: " << clients[val].get_hostname() << std::endl;
	return clientSocket;
}

// select() gives you the power to monitor several sockets at the same time
void Server::accept_connections()
{
	fd_set ready_sockets, wready_sockets;
	int clientSocket;

	FD_ZERO(&ready_sockets);
	FD_SET(server, &current_sockets);
	while (1)
	{
		// select changes the set passed in so we ned temporary copy
		ready_sockets = current_sockets;
		wready_sockets = current_sockets;
		if (select(maxfd + 1, &ready_sockets, &wready_sockets, NULL, NULL) < 0)
			throw(7);
		for (int i = 0; i < maxfd + 1; i++)
		{
			if (FD_ISSET(i, &ready_sockets))
			{
				if (i == server)
				{
					clientSocket = accept_new_connection(server);
					if (clientSocket != -1)
					{
						FD_SET(clientSocket, &current_sockets);
						if (clientSocket > maxfd)
						{
							maxfd = clientSocket;
						}
					}
				}
				else
				{
					handle_connection(i);
				}
			}
		}
	}
	close(server);
}

int Server::get_server()
{
	return server;
}

Server::Server(char **argv)
{
	int i = 0;
	while (argv[1][i])
	{
		if (!isdigit(argv[1][i]))
			throw 2;
		i++;
	}
	port = strtod(argv[1], NULL);
	if (port < 0 || port > 65353)
		throw 2;
	password = argv[2];
	FD_ZERO(&current_sockets);
}

std::string Server::get_pass()
{
	return password;
}

void Server::commandQuit(Client *member, const std::string reason)
{
	std::cout << "Connection closed!\n";
	for (itChan ite = channels.begin(); ite != channels.end(); ite++)
	{
		ite->second.memberQuit(member, reason);
	}
	if (auth_clients.find(member->get_nick()) != auth_clients.end())
	{
		for (std::map<std::string, Channel>::iterator i = channels.begin(); i != channels.end(); ++i)
		{
			i->second.change_in_all("", auth_clients.find(member->get_nick())->second, "LEAVE");
		}
		auth_clients.erase(member->get_nick());
		clients.erase(Server::to_string(member->get_fd()));
	}
	FD_CLR(member->get_fd(), &current_sockets);
	close(member->get_fd());
}