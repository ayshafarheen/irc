#include "IRC.hpp"

void Server::serv_handle(int n)
{
	if(n == 1)
		std::cerr << "Server error\n";
	if(n == 2)
		std::cerr << "Invalid port\n";
	if(n == 3)
		std::cerr << "Binding error\n";
	if(n == 4)
		std::cerr << "Queue size exceeded\n";
	if(n == 5)
		std::cerr << "Invalid client socket\n";
	if(n == 6)
		std::cerr << "receiving error\n";
	if(n == 7)
		std::cerr << "Select error\n";
	if(n == 8)
		std::cerr << "Invalid client socket\n";

}

void Server::set_server()
{
	try
	{
		server = socket(AF_INET, SOCK_STREAM, 0);
		if(server == -1)
			throw (1);
		this->serverAddr.sin_family = AF_INET;//IPV4
		this->serverAddr.sin_port = htons(port);//port number host to network
		this->serverAddr.sin_addr.s_addr = INADDR_ANY; //dont bind to particular ip but listen for all available IPs
		accept_connections();
	}
	catch(int n)
	{
		serv_handle(n);
	}
}

void Server::handle_connection(int clientSocket)
{
	int fail;

	Client client(clientSocket);
	// while(1)
	// {
		char buffer[1024] = {0};
		fail = recv(clientSocket, buffer, sizeof(buffer), 0);
		if(fail == -1)
			throw (6);
		if(fail == 0)
		{
			std::cout << "Connection closed! " << std::endl;
			// break;
		}
		*strchr(buffer, '\n') = 0;
		std::cout << "Message from client: " << buffer << std::endl;
	// }
}

int Server::accept_new_connection(int server, fd_set *ready_sockets)
{
	int clientSocket = accept(server, nullptr, nullptr);
	if(clientSocket == -1)
		throw (5);
	else
		FD_SET(clientSocket, ready_sockets);
	return clientSocket;
}

void Server::accept_connections()
{
	int fail;
	fail = bind(server, (const sockaddr*)&(this->serverAddr), sizeof(this->serverAddr));
	if(fail == -1)
		throw(3);
	fd_set current_sockets, ready_sockets;
	FD_ZERO(&current_sockets);
	FD_SET(server, &current_sockets);
	struct timeval tv;
	int clientSocket;
	tv.tv_sec = 2;
	tv.tv_usec = 500000;
	fail = listen(server, 5);

	while(1)
	{
		ready_sockets = current_sockets;
		if(select(FD_SETSIZE, &ready_sockets, NULL, NULL, NULL) < 0) {
			throw (7);
		}
			// std::cout << "here";
		if(fail == -1)
			throw(4);

		for(int i = 0; i< FD_SETSIZE; i++)
		{
			if(FD_ISSET(i,&ready_sockets))
			{
				if(i == server)
				{
					clientSocket = accept_new_connection(server, &ready_sockets);
					FD_SET(clientSocket, &current_sockets);
				}
				else
				{
					handle_connection(i);
					std::cout << "here";
					FD_CLR(i, &current_sockets);
				}
			}
		}
		clientSocket = accept_new_connection(server, &ready_sockets);
		// handle_connection(clientSocket);
	}
		std::cout << "here";
	close(server);

}

int Server::get_server()
{
	return server;
}

Server::Server(char **argv)
{
	int i = 0;
	while(argv[1][i])
	{
		if(!isdigit(argv[1][i]))
			throw 2;
		i++;
	}
	port = strtod(argv[1], NULL);
	if(port < 0 || port > 65353)
		throw 2;
	// std::cout << "here";
	password = argv[2];
}