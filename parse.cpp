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
	if (it != result.end() - 1)
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
	commandQuit(&client, args);
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
	std::string chan, str, pass;
	std::list<std::string> join;
	std::stringstream strm(args);
	itChan itr;
	while (!strm.eof())
	{
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
		// std::cout << join.front() << std::endl;
		itr = channels.find(chan);
		if (validChan(chan) == 1)
		{
			if (itr == channels.end())
			{
				channels[chan] = Channel(chan, &client);
			}
			if (pass.empty())
				channels[chan].addMember(&client, "");
			else
				channels[chan].addMember(&client, pass);

		}
		else
			client.send_msg(ERR_BADCHANMASK(client.get_nick(), client.get_servername()));
	}
}

std::string ft_strjoin_iterator(std::vector<std::string>::iterator &ite, std::vector<std::string> &vec)
{
	std::ostringstream oss;
	int i = 0;
	const char *delimiter = " ";

	for (; ite != vec.end(); ite++)
	{
		if (i > 0)
		{
			oss << delimiter;
		}
		oss << *ite;
		i++;
	}
	return oss.str();
}

// KICK #chatroom1 user123 :You are kicked!
void Server::command_kick_parsing(const std::string &args, Client &client)
{
	std::vector<std::string> vec = ft_split_whitespace(args);
	std::vector<std::string>::iterator ite;
	std::string chan_name;
	itChan channel;
	itCli useer;
	std::string user_k;
	std::string comments;

	if (vec.size() < 2)
		return client.send_msg(ERR_NEEDMOREPARAMS(client.get_nick(), "KICK", client.get_servername()));
	ite = vec.begin();
	chan_name = *ite;
	ite++;
	user_k = *ite;
	++ite;
	if (vec.size() >= 3)
		comments = ft_strjoin_iterator(ite, vec);
	std::cout << chan_name << " " << user_k << " " << comments << std::endl;

	// double check the channel if it is exist
	channel = channels.find(chan_name);
	if (channel->first != chan_name)
		return client.send_msg(ERR_NOSUCHCHANNEL(client.get_nick(), chan_name, client.get_servername()));
	useer = clients.find(user_k);
	if (useer->first != user_k)
		return client.send_msg(ERR_USERNOTINCHANNEL(client.get_nick(), user_k, chan_name));
	// calling the function
	channel->second.kickMember(&client, comments, &useer->second);
	// std::cout << "CHANNEL: " << channel->first << std::endl;
	// std::cout << "CHANNEL: " << channel->second.getKey() << std::endl;
}

// INVITE user123 #chatroom1
void Server::command_invite_parsing(const std::string &args, Client &client)
{
	std::vector<std::string> vec = ft_split_whitespace(args);
	std::vector<std::string>::iterator ite;
	std::string chan = vec[1];
	std::string to_invite = vec[0];
	itChan itr = channels.find(chan);
	itCli dest = auth_clients.find(to_invite);
	if (channels[chan].isOper(&client) == true)
	{
		if (itr == channels.end())
			return client.send_msg(ERR_NOSUCHCHANNEL(client.get_nick(), chan, client.get_servername()));
		else if (dest == clients.end())
			return client.send_msg(ERR_NOSUCHNICK(client.get_nick(), to_invite));
		else if ((channels[chan].isInChan(&dest->second) == true))
			return client.send_msg(ERR_USERONCHANNEL(client.get_nick(), client.get_nick(), chan, client.get_servername()));
		else
		{
			channels[chan].addToInvite(to_invite, &dest->second, &client);
			return dest->second.send_msg(RPL_INVITE(user_id(client.get_nick(), client.get_user(), client.get_servername()), to_invite, chan));
		}
	}
	client.send_msg(ERR_CHANOPRIVSNEEDED(client.get_nick(), chan, client.get_servername()));
}

// if they have none of them than it's just a wrong flag
bool check_mode(std::string args)
{
	if (args == "+i")
		return true;
	if (args == "-i")
		return true;
	if (args == "+t")
		return true;
	if (args == "-t")
		return true;
	if (args == "+k")
		return true;
	if (args == "-k")
		return true;
	if (args == "+o")
		return true;
	if (args == "-o")
		return true;
	if (args == "-l")
		return true;
	if (args == "+l")
		return true;
	return false;
}

// MODE #chatroom1 +o user123
void Server::command_mode_parsing(const std::string &args, Client &client)
{
	std::vector<std::string> vec = ft_split_whitespace(args);
	std::vector<std::string>::iterator ite;
	std::string chan;
	std::string user_m;
	std::string mode;
	itChan channel;
	itCli useer;

	if (vec.size() < 2)
		return client.send_msg(ERR_NEEDMOREPARAMS(client.get_nick(), "MODE", client.get_servername()));

	ite = vec.begin();
	chan = *ite;
	ite++;
	mode = *ite;
	++ite;

	channel = channels.find(chan);
	if (channel == channels.end())
		return client.send_msg(ERR_NOSUCHCHANNEL(client.get_nick(), chan, client.get_servername()));
	if (!channels[chan].isOper(&client))
		return client.send_msg(ERR_CHANOPRIVSNEEDED(client.get_nick(), chan, client.get_servername()));

	//
	// Mode specific
	//
	if (check_mode(mode) == false)
		return client.send_msg(ERR_UMODEUNKNOWNFLAG(client.get_nick(), client.get_servername()));

	// Only 2 params
	if (mode == "+i" || mode == "-i" || mode == "+t" || mode == "-t" || mode == "-k" || mode == "-l")
	{
		if (vec.size() > 2)
			return client.send_msg(ERR_UNKNOWNCOMMAND(client.get_nick(), "MODE", client.get_servername()));
		if (mode == "+i")
			channel->second.setInviteOnlyMode(client, true);
		if (mode == "-i")
			channel->second.setInviteOnlyMode(client, false);
		if (mode == "+t")
			channel->second.setTopicMode(client, true);
		if (mode == "-t")
			channel->second.setTopicMode(client, false);
		if (mode == "-k")
			channel->second.setPasswordNeededFalse(client);
		if (mode == "-l")
			channel->second.setUserLimit(client, -1);
		return;
	}

	// 3 params modes
	if (vec.size() < 3)
		return client.send_msg(ERR_NEEDMOREPARAMS(client.get_nick(), "MODE", client.get_servername()));
	if (vec.size() > 3)
		return client.send_msg(ERR_UNKNOWNCOMMAND(client.get_nick(), "MODE", client.get_servername()));

	// Takes password
	if (mode == "+k")
	{
		std::string password = *ite;

		//
		// TODO: need to check the password !!
		//

		return channel->second.setPasswordNeededTrue(client, password);
	}

	// Takes member
	if (mode == "+o" || mode == "-o")
	{
		user_m = *ite;
		useer = clients.find(user_m);
		if (useer->first != user_m)
			return client.send_msg(ERR_USERNOTINCHANNEL(client.get_nick(), user_m, chan));

		if (mode == "+o")
			channel->second.setPrivilageMode(&useer->second, true);
		if (mode == "-o")
			channel->second.setPrivilageMode(&useer->second, false);
		return;
	}

	// Takes limit
	if (mode == "+l")
	{
		int limit = atoi((*ite).c_str());

		//
		// TODO: Check the limit
		//

		channel->second.setUserLimit(client, limit);
		return;
	}
}

void handle_irssi(Client &client, std::vector<std::string> parts)
{
	if (parts.size() != 2)
	{
		client.send_msg(ERR_NEEDMOREPARAMS((client.get_nick().empty() ? "*" : client.get_nick()), "USER", client.get_servername()));
		return;
	}
	std::string username, hostname, servername, realname;
	std::vector<std::string> args_sp = ft_split_whitespace(parts[0]);
	if (!args_sp.empty() && (!args_sp[0].empty() && args_sp.size() == 3))
	{
		username = args_sp[0];
		if (!args_sp[1].empty())
		{
			hostname = args_sp[1];
			if (!args_sp[2].empty())
			{
				servername = args_sp[2];
				realname = parts[1];
			}
			else
			{
				client.send_msg(ERR_NEEDMOREPARAMS((client.get_nick().empty() ? "*" : client.get_nick()), "USER", client.get_servername()));
				return;
			}
		}
		else
		{
			client.send_msg(ERR_NEEDMOREPARAMS((client.get_nick().empty() ? "*" : client.get_nick()), "USER", client.get_servername()));
			return;
		}
	}
	else
	{
		client.send_msg(ERR_NEEDMOREPARAMS((client.get_nick().empty() ? "*" : client.get_nick()), "USER", client.get_servername()));
		return;
	}
	client.set_user(username);
	client.set_servername(servername);
	client.set_realname(realname);
}

void handle_nc(const std::string &args, Client &client)
{
	std::vector<std::string> args_sp = ft_split_whitespace(args);
	if (args_sp.size() != 1)
	{
		client.send_msg(ERR_NEEDMOREPARAMS((client.get_nick().empty() ? "*" : client.get_nick()), "USER", client.get_servername()));
		return;
	}
	else
		client.set_user(args_sp[0]);
}

void Server::command_user_parsing(const std::string &args, Client &client)
{
	if (client.get_nick() != "" && auth_clients.find(client.get_nick()) != auth_clients.end())
	{
		client.send_msg(ERR_ALREADYREGISTERED(client.get_nick(), client.get_servername()));
	}
	else
	{
		std::vector<std::string> parts = ft_split(args, ':');
		if (parts.size() == 1)
			handle_nc(args, client);
		else
			handle_irssi(client, parts);
		authenticate(client);
	}
}

void Server::command_topic_parsing(const std::string &args, Client &client)
{
	std::vector<std::string> args_sp = ft_split(args, ':');
	if (args_sp.size() == 2 || args_sp.size() == 1)
	{
		std::string channel = trim(std::string(args_sp[0]));
		if (channels.find(channel) == channels.end())
			client.send_msg(ERR_NOSUCHCHANNEL(client.get_nick(), args_sp[0], client.get_servername()));
		else if (!channels[channel].isInChan(&client))
			client.send_msg(ERR_NOTONCHANNEL(client.get_nick(), args_sp[0], client.get_servername()));
		// checking if the mode is restricted or no
		else if (channels[channel].getTopicMode() && channels[channel].isOper(&client) == false)
			client.send_msg(ERR_CHANOPRIVSNEEDED(client.get_nick(), channel, client.get_servername()));
		// havent got the correct reply but working
		else if (args_sp.size() == 2)
		{
			channels[channel].sendToAll(client,RPL_TOPIC_CHANGE(client.get_nick(), client.get_user(), channel, std::string(args_sp[1]), client.get_servername()), "TOPIC", true);
			channels[channel].setTopic(std::string(args_sp[1]));
		}
		else if (args_sp.size() == 1 && args.find(':'))
		{
			channels[channel].setTopic("");
			channels[channel].sendToAll(client,RPL_TOPIC_CHANGE(client.get_nick(), client.get_user(), channel, std::string(args_sp[1]), client.get_servername()), "TOPIC", true);
		}
		else if (args_sp.size() == 1)
		{
			if (channels[channel].getTopic().empty())
				client.send_msg(RPL_NOTOPIC(client.get_nick(), args_sp[0], client.get_servername()));
			else
				client.send_msg(RPL_TOPIC(client.get_nick(), args_sp[0], channels[channel].getTopic(), client.get_servername()));
		}
	}
	else
		client.send_msg(ERR_NEEDMOREPARAMS((client.get_nick()), "TOPIC", client.get_servername()));
}

void Server::command_nick_parsing(const std::string &args, Client &client)
{
	std::string nick = trim(args);
	if (!strchr(nick.c_str(), ' '))
	{
		if (auth_clients.find(nick) != auth_clients.end())
		{
			client.send_msg(ERR_NICKNAMEINUSE((client.get_nick().empty() ? "*" : client.get_nick()), nick, client.get_servername()));
		}
		else
		{
			if (nick.empty())
			{
				client.send_msg(ERR_NONICKNAMEGIVEN((client.get_nick().empty() ? "*" : client.get_nick()), client.get_servername()));
			}
			else if (Client::invalid_nick(nick))
			{
				client.send_msg(ERR_ERRONEUSNICKNAME((client.get_nick().empty() ? "*" : client.get_nick()), client.get_nick(), client.get_servername()));
			}
			else if (auth_clients.find(nick) == auth_clients.end())
			{

				if (!client.get_nick().empty())
				{
					std::string oldnick = client.get_nick();
					client.set_nick(nick);
					clients[client.get_nick()] = client;
					clients.erase(oldnick);
					auth_clients[client.get_nick()] = client;
					auth_clients.erase(oldnick);
					for (std::map<std::string, Channel>::iterator i = channels.begin(); i != channels.end(); ++i)
						((*i).second).change_in_all(oldnick, client, "NICK");
					client.send_msg(RPL_NICK(oldnick, client.get_user(), client.get_nick(), client.get_servername()));
				}
				else
				{
					client.set_nick(nick);
					authenticate(client);
				}
			}
			else
				client.send_msg(ERR_NICKNAMEINUSE((client.get_nick().empty() ? "*" : client.get_nick()), nick, client.get_servername()));
		}
	}
	else
		client.send_msg(ERR_ERRONEUSNICKNAME((client.get_nick().empty() ? "*" : client.get_nick()), client.get_nick(), client.get_servername()));
}

void Server::command_pass_parsing(const std::string &args, Client &client)
{
	if (auth_clients.find(client.get_nick()) == auth_clients.end())
	{
		if (args != Server::get_pass())
		{
			client.send_msg(ERR_PASSWDMISMATCH((client.get_nick().empty() ? "*" : client.get_nick()), client.get_servername()));
			FD_CLR(client.get_fd(), &current_sockets);
			close(client.get_fd());
			clients.erase(to_string(client.get_fd()));
			for (std::map<std::string, Channel>::iterator i = channels.begin(); i != channels.end(); ++i)
				((*i).second).change_in_all("", client, "LEAVE");
			throw(1);
		}
		else
		{
			client.set_auth(1);
			authenticate(client);
		}
	}
	else
		client.send_msg(ERR_ALREADYREGISTERED(client.get_nick(), client.get_servername()));
}

void Server::command_cap_parsing(const std::string &args, Client &client)
{
	if (first_word(args) == "LS")
	{
		client.send_msg("CAP * LS :multi-prefix sasl\r\n");
	}
	else if (first_word(args) == "REQ")
	{
		client.send_msg("CAP * ACK :multi-prefix\r\n");
	}
	else
		return;
}

void Server::command_ping_parsing(const std::string &args, Client &client)
{
	std::string token = args.substr(args.find(":") + 1);
	client.send_msg(RPL_PONG(client.get_servername(), token));
}

void Server::command_priv_parsing(const std::string &args, Client &client)
{
	std::vector<std::string> args_sp = ft_split(args, ':');
	if (args_sp.size() == 1)
	{
		if (args.find(':'))
		{
			if (args[args.length() - 1] == ':')
				client.send_msg(ERR_NOTEXTTOSEND(client.get_nick()));
			else
				client.send_msg(ERR_NORECIPIENT(client.get_nick()));
		}
	}
	else if (args_sp.size() == 2)
	{
		std::vector<std::string> receivers = ft_split(args_sp[0], ',');
		for (std::vector<std::string>::iterator i = receivers.begin(); i != receivers.end(); ++i)
		{
			if (auth_clients.find(trim(*i)) != auth_clients.end())
				auth_clients[trim(*i)].send_msg(RPL_PRIVMSG(client.get_nick(), client.get_user(), trim(*i), args_sp[1], client.get_servername()));
			else if (channels.find(trim(*i)) != channels.end())
			{
				if (!channels[trim(*i)].isInChan(&client))
					return client.send_msg(ERR_USERNOTINCHANNEL(client.get_nick(), client.get_nick(), channels[trim(*i)].getChanName()));

				channels[trim(*i)].sendToAll(client, RPL_PRIVMSG(client.get_nick(), client.get_user(), trim(*i), args_sp[1], client.get_servername()), "PRIVMSG", 1);
			}
			else
				client.send_msg(ERR_NOSUCHNICK(client.get_nick(), std::string(trim(*i))));
		}
	}
}

// void Server::command_priv_parsing(const std::string &args, Client &client)
// {
// 	std::vector<std::string> args_sp = ft_split(args, ':');
// 	if(args_sp.size() == 1)
// 	{
// 		if(args.find(':'))
// 		{
// 			if(args[args.length() - 1] == ':')
// 				client.send_msg(ERR_NOTEXTTOSEND(client.get_nick()));
// 			else
// 				client.send_msg(ERR_NORECIPIENT(client.get_nick()));
// 		}
// 	}
// 	else if(args_sp.size() == 2)
// 	{
// 		std::vector<std::string> receivers = ft_split(args_sp[0], ',');
// 		for (std::vector<std::string>::iterator i = receivers.begin(); i != receivers.end(); ++i)
// 		{
// 			std::string msg = client.get_nick() + ": " + args_sp[1] + "\r\n";
// 			if(auth_clients.find(trim(*i)) != auth_clients.end())
// 				auth_clients[trim(*i)].send_msg(msg);
// 			else if(channels.find(trim(*i)) != channels.end())
// 				channels[trim(*i)].sendToAll(client, msg, "PRIVMSG", 1);
// 			else
// 				client.send_msg(ERR_NOSUCHNICK(client.get_nick(),std::string(trim(*i))));
// 		}
// 	}
// }

void Server::command_motd_parsing(const std::string &args, Client &client)
{
	(void)args;

	std::fstream msg_file;
	std::string msg;
	msg_file.open("MOTD.txt");
	if (msg_file.is_open())
	{
		client.send_msg(RPL_MOTDSTART(client.get_nick(), client.get_servername()));
		while (getline(msg_file, msg))
		{
			client.send_msg(RPL_MOTD(client.get_nick(), msg, client.get_servername()));
		}
		client.send_msg(RPL_ENDOFMOTD(client.get_nick(), client.get_servername()));
	}
	else
		client.send_msg(ERR_NOMOTD(client.get_nick()));
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
	if (auth_clients.find(client.get_nick()) != auth_clients.end())
	{
		commandMap.insert(std::make_pair("motd", &Server::command_motd_parsing));
		commandMap.insert(std::make_pair("JOIN", &Server::command_join_parsing));
		commandMap.insert(std::make_pair("TOPIC", &Server::command_topic_parsing));
		commandMap.insert(std::make_pair("KICK", &Server::command_kick_parsing));
		commandMap.insert(std::make_pair("INVITE", &Server::command_invite_parsing));
		commandMap.insert(std::make_pair("MODE", &Server::command_mode_parsing));
		commandMap.insert(std::make_pair("PING", &Server::command_ping_parsing));
		commandMap.insert(std::make_pair("PART", &Server::command_ping_parsing));
		commandMap.insert(std::make_pair("PRIVMSG", &Server::command_priv_parsing));
	}
	std::vector<std::string> commands = ft_split(clientmsg, '\n');
	try
	{
		for (unsigned long i = 0; i < commands.size(); i++)
		{
			//  std::cout << "Debug: commands[" << i << "] = " << commands[i] << std::endl;
			std::cout << "Command is: " << commands[i] << std::endl;

			command_name = first_word(commands[i]);
			if (command_name.empty())
			{
				return;
			}

			if (commandMap.find(command_name) != commandMap.end())
			{
				(this->*(commandMap[command_name]))(trim(commands[i].substr(command_name.length())), client);
			}
			else if (!client.get_nick().empty() && auth_clients.find(client.get_nick()) == auth_clients.end())
			{
				std::cerr << "Error: Unsupported command" << std::endl;
			}
		}
	}
	catch (int n)
	{
		(void)n;
	}
}
