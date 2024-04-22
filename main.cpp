#include "IRC.hpp"

int main()
{
    int server = socket(AF_INET, SOCK_STREAM, 0);
    std::cout << "boo " << server;
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;//IPV4
    serverAddr.sin_port = htons(8080);//port number host to network
    serverAddr.sin_addr.s_addr = INADDR_ANY; //dont bind to particular ip but listen for all available IPs
    bind(server, (const sockaddr*)&serverAddr, sizeof(serverAddr));
    listen(server, 5);
}
