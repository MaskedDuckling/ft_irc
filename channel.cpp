#include "channel.hpp"

channel::channel(){
}

channel::channel(std::string name, user *user):_name(name){
    _users.push_back(user);
    _users.back()->_channel = this;
}

channel::~channel(){
}

void channel::broadcast(std::string response){
    for (std::vector <user *>::iterator it = _users.begin(); it != _users.end(); it++)
        send((*it)->_fd, response.c_str(), response.size(), 0);
}