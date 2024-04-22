#include "IRC.hpp"

void Server::set_server()
{
	server = socket(AF_INET, SOCK_STREAM, 0);
	// clients.push_back(socket(AF_INET, SOCK_STREAM, 0));
	this->serverAddr.sin_family = AF_INET;//IPV4
    this->serverAddr.sin_port = htons(port);//port number host to network
    this->serverAddr.sin_addr.s_addr = INADDR_ANY; //dont bind to particular ip but listen for all available IPs
	accept_connections();
}

void Server::accept_connections()
{
	bind(server, (const sockaddr*)&(this->serverAddr), sizeof(this->serverAddr));
	listen(server, 5);
	// connect(clients[0], (struct sockaddr*)&serverAddr, sizeof(serverAddr));
	int clientSocket = accept(server, nullptr, nullptr);
	char buffer[1024] = {0};
	recv(clientSocket, buffer, sizeof(buffer), 0);
	std::cout << "Message from client: " << buffer << std::endl;
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
		{
			std::cerr << "Input error!\n";
			exit(0);
		}
		i++;
	}
	port = strtod(argv[1], NULL);
	password = argv[2];
}

// int main(int argc, char **argv)
// {
// 	if(argc == 3)
// 	{
// 		Server serv(argv);
// 		serv.set_server();
// 	}
// }