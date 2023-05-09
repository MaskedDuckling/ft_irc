#ifndef CHANNEL_HPP
# define CHANNEL_HPP

#include <vector>
#include "user.hpp"

class user;

class channel{
    private:
        std::string _name;
        std::vector <user *> _users;
    public:
        channel();
        channel(std::string name, user *user);
        ~channel();

};

#endif