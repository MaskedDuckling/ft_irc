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
    std::string str = user->_nick;
    for (std::list <std::string>::iterator it = _history.begin(); it != _history.end(); it++)
        send(user->_fd, it->c_str(), it->size(), 0);
    str += "\033[0;33m has joined the channel: \033[0m";
    str += _name;
    str += "\n";
    broadcast(str);
    if (_history.size() > 20)
        _history.pop_front();
    _history.push_back(str);
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

void channel::print_msg(std::vector<std::string> str, user *user)
{
    std::string name;
    name = user->_nick;
    name += " : ";
    for (std::vector <std::string>::iterator it = str.begin(); it != str.end(); it++)
    {
        name += *it;
        name += " ";
    }
    name += "\n";
    if (_history.size() > 20)
        _history.pop_front();
    _history.push_back(name);
    broadcast(name);
}