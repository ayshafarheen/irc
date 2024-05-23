#include "IRC.hpp"

std::string Server::password = "";
int Server::server = 0;

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

void Server::authenticate(Client client)
{
	if(client.get_user() != "" && client.get_nick() != "" && client.get_auth())
	{
		auth_clients[client.get_nick()] = clients[std::to_string(client.get_fd())];
		char _hostname[1024];
		std::string hostname;
		gethostname(_hostname, 1024);
		hostname = (std::string)_hostname;
		time_t now = time(0);
		char* date_time = ctime(&now);
		std::string id = client.get_nick() + "@" + hostname;
		client.send_msg(RPL_WELCOME(id, client.get_nick()));
		client.send_msg(RPL_YOURHOST(client.get_nick(), hostname, "1.0"));
		client.send_msg(RPL_CREATED(client.get_nick(), date_time));
	}
}

void Server::handle_connection(int clientSocket)
{
	int fail;
	// int count, total;
	// total = 0;

	char buffer[1024] = {0};
// 	while ((count = recv(clientSocket, &buffer[total], sizeof(buffer) - total, 0)) > 0)
// {
//     total += count;
// 	// std::cout << buffer;
//     // At this point the buffer is valid from 0..total-1, if that's enough then process it and break, otherwise continue
// 	memset(buffer,0,1024);
// }
// if (count == -1)
// {
//     perror("recv");
// }
// else if (count == 0)
// {
//     // EOS on the socket: close it, exit the thread, etc.
// }
	fail = recv(clientSocket, buffer, sizeof(buffer), 0);
	if (fail == -1)
		throw(6);
	if (fail == 0)
	{
		std::cout << "Connection closed! " << std::endl;
		FD_CLR(clientSocket, &current_sockets);
		return;
	}
	Client &client = clients[std::to_string(clientSocket)];
	client.set_msg(buffer);
	parse_and_execute_client_command(client.get_msg(), client);
	// msg = buffer;
}

int Server::accept_new_connection(int server)
{
	int clientSocket = accept(server, NULL, NULL);
	 fcntl(clientSocket, F_SETFL, O_NONBLOCK);
	Client client1(clientSocket);
	if (clientSocket == -1)
		throw(5);
	if (clientSocket > maxfd)
		maxfd = clientSocket;
	std::string val = std::to_string(clientSocket);
	std::cout << "ADED!!\n";
	clients[val] = client1;
	return clientSocket;
}

// select() gives you the power to monitor several sockets at the same time
void Server::accept_connections()
{
	fd_set ready_sockets;
	int clientSocket;

	FD_ZERO(&ready_sockets);
	FD_SET(server, &current_sockets);
	while (1)
	{
		// select changes the set passed in so we ned temporary copy
		ready_sockets = current_sockets;
		if (select(maxfd +1, &ready_sockets, NULL, NULL, NULL) < 0)
			throw(7);
		for (int i = 0; i < maxfd +1; i++)
		{
			if (FD_ISSET(i, &ready_sockets))
			{
				if (i == server)
				{
					clientSocket = accept_new_connection(server);
					FD_SET(clientSocket, &current_sockets);
				}
				else
				{
					handle_connection(i);
					break;
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
	for (itChan ite = channels.begin(); ite != channels.end(); ite++)
	{
		ite->second.memberQuit(member, reason);
	}
}