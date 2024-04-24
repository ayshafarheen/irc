#include "IRC.hpp"

int main(int argc, char **argv)
{
	if(argc == 3)
	{
		try
		{
			Server serv(argv);
			serv.set_server();
		}
		catch(int n)
		{
			Server::serv_handle(n);
		}
	}
}
