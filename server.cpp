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
		client.send_msg(RPL_WELCOME(std::to_string(client.get_fd()), client.get_nick()));
	}
}

void Server::handle_connection(int clientSocket)
{
	int fail;

	char buffer[1024] = {0};
	fail = recv(clientSocket, buffer, sizeof(buffer), 0);
	if (fail == -1)
		throw(6);
	if (fail == 0)
	{
		std::cout << "Connection closed! " << std::endl;
		FD_CLR(clientSocket, &current_sockets);
		return;
	}
	// *strchr(buffer, '\n') = 0;
	msg = buffer;
	Client &client = clients[std::to_string(clientSocket)];
	client.set_msg(buffer);
	// std::cout << "Message from client " << client.get_fd() << ": " << client.get_msg()  << std::endl;
	parse_and_execute_client_command(client.get_msg(), client);
}

int Server::accept_new_connection(int server)
{
	int clientSocket = accept(server, NULL, NULL);
	Client client1(clientSocket);
	if (clientSocket == -1)
		throw(5);
	if (clientSocket > maxfd)
		maxfd = clientSocket;
	std::string val = std::to_string(clientSocket);
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
		if (select(FD_SETSIZE, &ready_sockets, NULL, NULL, NULL) < 0)
			throw(7);
		for (int i = 0; i < FD_SETSIZE; i++)
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
