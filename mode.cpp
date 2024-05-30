#include "Channel.hpp"
#include "IRC.hpp"

// enabling the client with the mode

// + to set and - to remove
// + is true and - is false

void Channel::setInviteOnlyMode(Client &client, bool enable)
{
    this->inviteOnly = enable;
	if (enable == true)
		sendToAll(client, MODE_CHANNELMSG(this->getChanName(), "+i", client.get_servername()), "MODE", true);
	else
		sendToAll(client, MODE_CHANNELMSG(this->getChanName(), "-i", client.get_servername()), "MODE", true);
}

void Channel::setPasswordNeeded(Client &client, bool enable)
{
    this->passwordNeeded = enable;
    if (enable == true)
		sendToAll(client, MODE_CHANNELMSG(this->getChanName(), "+k", client.get_servername()), "MODE", true);
	else
		sendToAll(client, MODE_CHANNELMSG(this->getChanName(), "-k", client.get_servername()), "MODE", true);
}

void Channel::setPrivilageMode(Client *member, bool enable)
{
    if (enable == true)
    {
        if (opers.find(member->get_nick()) != opers.end())
        {
            std::cout << "already operator " << member->get_nick() << std::endl;
            return;
        }
        opers.insert(std::pair<std::string, Client *>(member->get_nick(), member));
        //member->send_msg(RPL_YOUREOPER(member->get_nick()));
        this->sendToAll(*member, MODE_CHANNELMSGWITHPARAM(getChanName(), "+o", member->get_nick(), member->get_servername()), "MODE", true);
    }
    else
    {
        if (opers.find(member->get_nick()) == opers.end())
            return;
        opers.erase(member->get_nick());
        this->sendToAll(*member, MODE_CHANNELMSGWITHPARAM(getChanName(), "-o", member->get_nick(), member->get_servername()), "MODE", true);
    }
}

void Channel::setTopicMode(Client &client, bool enable)
{
    this->topicMode = enable;
    if (enable == true)
		sendToAll(client, MODE_CHANNELMSG(this->getChanName(), "+t", client.get_servername()), "MODE", true);
	else
		sendToAll(client, MODE_CHANNELMSG(this->getChanName(), "-t", client.get_servername()), "MODE", true);
}

void Channel::setLimitMember(Client &client, bool enable)
{
    this->limitMode = enable;
    if (enable == true)
		sendToAll(client, MODE_CHANNELMSG(this->getChanName(), "+l", client.get_servername()), "MODE", true);
	else
		sendToAll(client, MODE_CHANNELMSG(this->getChanName(), "-l", client.get_servername()), "MODE", true);
}