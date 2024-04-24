#include "IRC.hpp"

void Client::create_client()
{
	int client = socket(AF_INET, SOCK_STREAM, 0);
	sockaddr_in serverAddress;
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(8080);
	serverAddress.sin_addr.s_addr = INADDR_ANY;
	connect(client, (struct sockaddr*)&serverAddress, sizeof(serverAddress));
	const char* message = "Hello, server!";
	send(client, message, strlen(message), 0);
	close(client);
}

Client::Client(int socket)
{
	fd = socket;
}

// int main(int argc, char **argv)
// {

// 		Client::create_client();
// }