#include "IRC.hpp"

std::string trim(const std::string &str)
{
    std::string result = str;
    std::string::iterator it = result.begin();

    while (it != result.end() && (*it == ' ' || *it == '\t'))
    {
        ++it;
    }
    result.erase(result.begin(), it);
    return result;
}

std::vector<std::string> ft_split_whitespace(const std::string &str)
{
    std::vector<std::string> words;
    std::istringstream iss(str);
    std::string word;

    while (iss >> word)
    {
        words.push_back(word);
    }
    return words;
}

std::vector<std::string> ft_split(const std::string &str, char delimiter)
{
    std::vector<std::string> tokens;
    std::istringstream iss(str);
    std::string token;

    while (std::getline(iss, token, delimiter))
    {
        tokens.push_back(token);
    }

    return tokens;
}

std::string first_word(const std::string &str)
{
    std::istringstream iss(str);
    std::string word;

    iss >> word;

    return word;
}

// QUIT :Leaving IRC
void Server::command_quit_parsing(const std::string &args, Client &client)
{
    std::cout << "PARSE [" << args << "]" << std::endl;
	(void)client;
}

// JOIN #chatroom1,#chatroom2
void Server::command_join_parsing(const std::string &args, Client &client)
{
    std::string chan, str, pass;
    std::list<std::string> join;
    std::stringstream strm(args);
    strm.str(str);
    while (!strm.eof()){
        std::getline(strm, chan, ',');
        join.push_back(chan);
    }
    if (args.size() > 1)
    {
        pass = args[1];
    }
    while (!join.empty())
    {
        chan = join.back();
        
    }
    
    // (void)args;
	(void)client;
}

// KICK #chatroom1 user123 :You are kicked!
void Server::command_kick_parsing(const std::string &args, Client &client)
{
    (void)args;
	(void)client;
}

// INVITE user123 #chatroom1
void Server::command_invite_parsing(const std::string &args, Client &client)
{
    (void)args;
	(void)client;
}

// MODE #chatroom1 +o user123
void Server::command_mode_parsing(const std::string &args, Client &client)
{
    (void)args;
	(void)client;
}

void Server::command_user_parsing(const std::string &args, Client &client)
{
	if (client.get_nick() != "" && auth_clients.find(client.get_nick()) != auth_clients.end())
	{
		client.send_msg(ERR_ALREADYREGISTERED(client.get_nick()));
	}
	else
	{
		client.set_user(trim(args));
		authenticate(client);
	}
}

// int invalid_nick(std::string nick)
// {

// }


void Server::command_nick_parsing(const std::string &args, Client &client)
{
	std::string nick = trim(args);
	if(nick == "")
		client.send_msg(ERR_NONICKNAMEGIVEN(client.get_nick()));
	else if(Client::invalid_nick(nick))
		client.send_msg( ERR_ERRONEUSNICKNAME(client.get_user(), client.get_nick()));
    else if (auth_clients.find(nick) == auth_clients.end())
    {
        client.set_nick(nick);
        authenticate(client);
    }
    else {
        clients.erase(std::to_string(client.get_fd()));
		FD_CLR(client.get_fd(), &current_sockets);
		client.send_msg(ERR_NICKNAMEINUSE(client.get_user(), client.get_nick()));
		close(client.get_fd());
    }
}

void Server::command_pass_parsing(const std::string &args, Client &client)
{
	if(args != Server::get_pass())
	{
		clients.erase(std::to_string(client.get_fd()));
		FD_CLR(client.get_fd(), &current_sockets);
		const char* message = "Incorrect password!\n";
		send(client.get_fd(), message, strlen(message), 0);
		close(client.get_fd());
	}
	else
	{
		client.set_auth(1);
		authenticate(client);
	}
}

// Typedef for function pointers
typedef void (Server::*CommandFunction)(const std::string &args, Client &client);


void Server::parse_and_execute_client_command(const std::string &clientmsg, Client &client)
{
    std::vector<std::string> words;
    std::string command_name;
    std::map<std::string, CommandFunction> commandMap;

    commandMap.insert(std::make_pair("QUIT", &Server::command_quit_parsing));
    commandMap.insert(std::make_pair("JOIN", &Server::command_join_parsing));
    commandMap.insert(std::make_pair("KICK", &Server::command_kick_parsing));
    commandMap.insert(std::make_pair("INVITE", &Server::command_invite_parsing));
    commandMap.insert(std::make_pair("MODE", &Server::command_mode_parsing));
	commandMap.insert(std::make_pair("USER", &Server::command_user_parsing));
    commandMap.insert(std::make_pair("NICK", &Server::command_nick_parsing));
	commandMap.insert(std::make_pair("PASS", &Server::command_pass_parsing));

    command_name = first_word(clientmsg);
    if (command_name.empty())
    {
        // No command / invalid
        // TODO: print something
        return;
    }
    // Call the function associated with the command name
    if (commandMap.find(command_name) != commandMap.end())
    {
        (this->*(commandMap[command_name]))(trim(clientmsg.substr(command_name.length())), client);
		std::cout << "Client ";
		std::cout << client;
    }
    else
    {
        // TODO: change the message
        std::cerr << "Error: Unsupported command" << std::endl;
    }
}