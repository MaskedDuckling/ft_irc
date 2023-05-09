#include "channel.hpp"

channel::channel(){
}

channel::channel(std::string name, user *user):_name(name){
    _users.push_back(user);
}

channel::~channel(){
}
