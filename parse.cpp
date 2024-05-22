#include "IRC.hpp"

std::string trim(const std::string &str)
{
    std::string result = str;
    std::string::iterator it = result.begin();

    while (it != result.end() && isspace(*it))
    {
        ++it;
    }
    result.erase(result.begin(), it);
	it = result.end() - 1;
	while (it != result.begin() && isspace(*it))
    {
        it--;
    }
	if(it != result.end() - 1)
		result.erase(it + 1, result.end());
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

int validChan(std::string channame)
{
    if (channame.find("#") == 0)
        return (1);
	else if (channame.find("!") == 0)
		return (1);
	else if (channame.find("&") == 0)
		return (1);
	else if (channame.find("+") == 0)
		return (1);
    return (0);
}
// JOIN #chatroom1,#chatroom2
void Server::command_join_parsing(const std::string &args, Client &client)
{
	std::string test = args;
	if(test.length() == 0)
	{
		std::cout << "write something" << std::endl;
		return ;
	}
    std::string chan, str, pass;
    std::list<std::string> join;
    std::stringstream strm(args);
    itChan itr;
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
        join.pop_back();
		itr = channels.find(chan);
        if (validChan(chan) == 1)
        {
            if (itr == channels.end())
             {
                channels[chan] = Channel(chan, &client);
             }
			else if (!(channels[chan].getUsrLim() > 0) && (channels[chan].getSize() >= channels[chan].getUsrLim()))
				client.send_msg(ERR_CHANNELISFULL(client.get_nick(),chan));
			if (channels[chan].getKey() != pass)
				client.send_msg(ERR_BADCHANNELKEY(client.get_nick(), chan));
			else
				channels[chan].addMember(&client);
        }
        else
            client.send_msg(ERR_BADCHANNELKEY(client.get_nick(), chan));

    }
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
		std::vector<std::string> parts = ft_split(args, ':');
		if(parts.size() != 2)
		{
			client.send_msg(ERR_NEEDMOREPARAMS(client.get_nick(), "USER"));
			return ;
		}
		std::string username, hostname, servername, realname;
		std::vector<std::string> args_sp = ft_split(parts[0], ' ');
		if(!args_sp[0].empty())
		{
			username = args_sp[0];
			if(!args_sp[1].empty())
			{
				hostname = args_sp[1];
				if(!args_sp[2].empty())
				{
					servername = args_sp[2];
					realname = parts[1];
					if(!args_sp[3].empty()){
			std::cout << "??\n";
						client.send_msg(ERR_NEEDMOREPARAMS(client.get_nick(), "USER"));}
				}
				else
					client.send_msg(ERR_NEEDMOREPARAMS(client.get_nick(), "USER"));
			}
			else
				client.send_msg(ERR_NEEDMOREPARAMS(client.get_nick(), "USER"));
		}
		client.set_user(username);
		client.set_servername(servername);
		client.set_realname(realname);
		authenticate(client);
	}
}


void Server::command_nick_parsing(const std::string &args, Client &client)
{
	std::string nick = args;
	if(nick.empty())
	{
		client.send_msg(ERR_NONICKNAMEGIVEN(client.get_nick()));
	}
	else if(Client::invalid_nick(nick))
	{
		client.send_msg( ERR_ERRONEUSNICKNAME(client.get_user(), client.get_nick()));
	}
    else if (auth_clients.find(nick) == auth_clients.end())
    {
        client.set_nick(nick);
        authenticate(client);
    }
    else {
        clients.erase(std::to_string(client.get_fd()));
		FD_CLR(client.get_fd(), &current_sockets);
		client.send_msg(ERR_NICKNAMEINUSE(client.get_user(), client.get_nick()));
    }
}

void Server::command_pass_parsing(const std::string &args, Client &client)
{
	if(args != Server::get_pass())
	{
		clients.erase(std::to_string(client.get_fd()));
		FD_CLR(client.get_fd(), &current_sockets);
		client.send_msg("Incorrect password!\n");
		std::cout << "password : " << args << " " << args.length() << "!" << std::endl;
		std::cout << "password : " << Server::get_pass() << " " << Server::get_pass().length() << "!" << std::endl;
	}
	else
	{
		client.set_auth(1);
		authenticate(client);
	}
}

void Server::command_cap_parsing(const std::string &args, Client &client)
{
	if(first_word(args) == "LS")
	{
		client.send_msg("CAP * LS :multi-prefix sasl\r\n");
	}
	else if(first_word(args) == "REQ")
	{
		client.send_msg("CAP * ACK :multi-prefix\r\n");
	}
	else
		return ;
}

void Server::command_ping_parsing(const std::string &args, Client &client)
{
	(void) args;
	client.send_msg("PONG");
}

// Typedef for function pointers
typedef void (Server::*CommandFunction)(const std::string &args, Client &client);


void Server::parse_and_execute_client_command(const std::string &clientmsg, Client &client)
{
    std::vector<std::string> words;
    std::string command_name;
    std::map<std::string, CommandFunction> commandMap;

	commandMap.insert(std::make_pair("CAP", &Server::command_cap_parsing));
	commandMap.insert(std::make_pair("USER", &Server::command_user_parsing));
    commandMap.insert(std::make_pair("NICK", &Server::command_nick_parsing));
	commandMap.insert(std::make_pair("PASS", &Server::command_pass_parsing));
    commandMap.insert(std::make_pair("QUIT", &Server::command_quit_parsing));
	if(client.get_auth())
	{
    	commandMap.insert(std::make_pair("JOIN", &Server::command_join_parsing));
		commandMap.insert(std::make_pair("KICK", &Server::command_kick_parsing));
		commandMap.insert(std::make_pair("INVITE", &Server::command_invite_parsing));
		commandMap.insert(std::make_pair("MODE", &Server::command_mode_parsing));
		commandMap.insert(std::make_pair("PING", &Server::command_ping_parsing));
		commandMap.insert(std::make_pair("PART", &Server::command_ping_parsing));

	}
	std::vector<std::string> commands = ft_split(clientmsg, '\n');
	for (unsigned long i = 0; i < commands.size() ; i++)
	{
		std::cout << "Command is:  " << commands[i] << std::endl;
		command_name = first_word(commands[i]);
		if (command_name.empty())
		{
			// No command / invalid
			// TODO: print something
			return;
		}
		// Call the function associated with the command name
		if (commandMap.find(command_name) != commandMap.end())
		{
			(this->*(commandMap[command_name]))(trim(commands[i].substr(command_name.length())), client);
		}
		else if(!client.get_nick().empty() && auth_clients.find(client.get_nick()) == auth_clients.end())
		{
			// TODO: change the message
			std::cerr << "Error: Unsupported command" << std::endl;
		}
	}
}
