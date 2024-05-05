#include "IRC.hpp"

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
void command_quit_parsing(const std::string &args)
{
    std::cout << "PARSE [" << args << "]" << std::endl;
}

// JOIN #chatroom1,#chatroom2
void command_join_parsing(const std::string &args)
{
    (void)args;
}

// KICK #chatroom1 user123 :You are kicked!
void command_kick_parsing(const std::string &args)
{
    (void)args;
}

// INVITE user123 #chatroom1
void command_invite_parsing(const std::string &args)
{
    (void)args;
}

// MODE #chatroom1 +o user123
void command_mode_parsing(const std::string &args)
{
    (void)args;
}

// Typedef for function pointers
typedef void (*CommandFunction)(const std::string &args);

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

void parse_and_execute_client_command(const std::string &clientmsg)
{
    std::vector<std::string> words;
    std::string command_name;
    std::map<std::string, CommandFunction> commandMap;

    commandMap.insert(std::make_pair("QUIT", command_quit_parsing));
    commandMap.insert(std::make_pair("JOIN", command_join_parsing));
    commandMap.insert(std::make_pair("KICK", command_kick_parsing));
    commandMap.insert(std::make_pair("INVITE", command_invite_parsing));
    commandMap.insert(std::make_pair("MODE", command_mode_parsing));

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
        commandMap[command_name](trim(clientmsg.substr(command_name.length())));
    }
    else
    {
        // TODO: change the message
        std::cerr << "Error: Unsupported command" << std::endl;
    }
}