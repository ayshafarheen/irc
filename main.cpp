#include "IRC.hpp"

int main(int argc, char **argv)
{
	if(argc == 3)
	{
		Server serv(argv);
		serv.set_server();
		// Client::create_client();
	}
}
