#ifndef CHANNEL_HPP
# define CHANNEL_HPP

#include <vector>
#include "user.hpp"

class user;

class channel{
    private:
    public:
        std::string _name;
        std::vector <user *> _users;

        channel();
        channel(std::string name, user *user);
        ~channel();

        void broadcast(std::string response);
};

#endif