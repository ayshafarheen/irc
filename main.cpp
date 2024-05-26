#include "IRC.hpp"
Server * class_ptr = NULL;

void signal_handler(int signal_num)
{
    // std::cout << "Interrupt signal is (" << signal_num << ").\n";
	for (std::map<std::string,Client>::iterator i = class_ptr->get_client().begin(); i != class_ptr->get_client().end(); ++i)
	{
		std::cout << ((*i).second).get_fd() << std::endl;
		close((((*i).second)).get_fd());
	}
	close(class_ptr->get_server());
	class_ptr->clear_all();
    exit(signal_num);
}
int main(int argc, char **argv)
{
	signal(SIGINT, signal_handler);
	signal(SIGQUIT, signal_handler);
	Server serv(argv);
	class_ptr = &serv;
	if(argc == 3)
	{
		try
		{
			serv.set_server();
		}
		catch(int n)
		{
			Server::serv_handle(n);
		}
	}
}
