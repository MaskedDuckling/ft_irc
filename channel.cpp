#include "channel.hpp"

channel::channel(){
}

channel::channel(std::string name, user *user, serveur *serv):_name(name), _serv(serv){
    add_user(user);
}

channel::~channel(){
}

void channel::add_user(user *user){
    _users.push_back(user);
    _users.back()->_mode = 2;
    user->_channel.push_back(this);
    user->_commands.back()->display_reply(CLEAR_TERM);
    std::string str = user->_nick;
    str += "\033[0;33m has joined the channel: \033[0m";
    str += _name;
    str += "\n";
    broadcast(str);
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