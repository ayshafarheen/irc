#include "Channel.hpp"
#include "IRC.hpp"

// enabling the client with the mode

// + to set and - to remove
// + is true and - is false

void Channel::setInviteOnlyMode(bool enable)
{
    this->inviteOnly = enable;
}

void Channel::setPasswordNeeded(bool enable)
{
    this->passwordNeeded = enable;
}

void Channel::setPrivilageMode(Client *member, bool enable)
{
    if (enable == true)
    {
        if (opers.find(member->get_nick()) != opers.end())
            return;
        member->set_oper(true);
        opers.insert(std::pair<std::string, Client *>(member->get_nick(), member));
        member->send_msg(RPL_YOUREOPER(member->get_nick()));
    }
    else
    {
        if (opers.find(member->get_nick()) == opers.end())
            return;
        opers.erase(member->get_nick());
        // TODO: send message not an operator anymore
    }

}

void Channel::setTopicMode(bool enable)
{
    this->topicMode = enable;
}