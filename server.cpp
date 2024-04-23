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

void Server::accept_connections()
{
	int fail;
	fail = bind(server, (const sockaddr*)&(this->serverAddr), sizeof(this->serverAddr));
	if(fail == -1)
		throw(3);
	while(1)
	{
		fail = listen(server, 5);
		if(fail == -1)
			throw(4);
		int clientSocket = accept(server, nullptr, nullptr);
		if(clientSocket == -1)
			throw (5);
		char buffer[1024] = {0};
		recv(clientSocket, buffer, sizeof(buffer), 0);
		std::cout << "Message from client: " << buffer << std::endl;
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
	while(argv[1][i])
	{
		if(!isdigit(argv[1][i]))
			throw 2;
		i++;
	}
	if(port >= 0 && port <= 65353)
		port = strtod(argv[1], NULL);
	else
		throw 2;
	password = argv[2];
}