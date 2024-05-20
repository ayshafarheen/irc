#include "IRC.hpp"
// #include <limits.h>
// #include <stdio.h>
// #include <stdlib.h>
// #include <unistd.h>
// #include <iostream>
//   #include <sys/types.h>
//        #include <sys/socket.h>
//        #include <netdb.h>
// #include <netdb.h>

int main()
{
	char hostname[1024];


	// int sd, err;
    // struct addrinfo hints = {}, *addrs;
    // char port_str[16] = {};

    // hints.ai_family = AF_INET; // Since your original code was using sockaddr_in and
    //                            // PF_INET, I'm using AF_INET here to match.  Use
    //                            // AF_UNSPEC instead if you want to allow getaddrinfo()
    //                            // to find both IPv4 and IPv6 addresses for the hostname.
    //                            // Just make sure the rest of your code is equally family-
    //                            // agnostic when dealing with the IP addresses associated
    //                            // with this connection. For instance, make sure any uses
    //                            // of sockaddr_in are changed to sockaddr_storage,
    //                            // and pay attention to its ss_family field, etc...
    // hints.ai_socktype = SOCK_STREAM;
    // hints.ai_protocol = IPPROTO_TCP;
	// hints.ai_flags = AI_CANONNAME;

    // err = getaddrinfo(NULL, NULL, &hints, &addrs);
	// std::cout << addrs->ai_canonname;
	struct hostent *he;
	he = gethostbyname ("localhost");
	std::cout << he->;
}