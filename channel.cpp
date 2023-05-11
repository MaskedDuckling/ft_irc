#include "channel.hpp"

channel::channel(){
}

channel::channel(std::string name, user *user):_name(name){
    add_user(user);
}

channel::~channel(){
}

void channel::add_user(user *user){
    _users.push_back(user);
    _users.back()->_mode = 2;
    _users.back()->_channel = this;
}

void channel::broadcast(std::string response){
    for (std::vector <user *>::iterator it = _users.begin(); it != _users.end(); it++)
        send((*it)->_fd, response.c_str(), response.size(), 0);
}

void channel::list_users(){
    int users = 0;
    for (std::vector<user *>::iterator it = _users.begin(); it != _users.end(); it++){
        std::cout << (*it)->_nick << std::endl;
        users++;
    }
    std::cout << users << " users in channel " << std::endl;
}