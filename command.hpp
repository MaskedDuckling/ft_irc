#ifndef COMMAND_HPP
# define COMMAND_HPP

#include <vector>
#include <iostream>
#include "user.hpp"

class user;

class command{
    // private:
    public:
        std::vector<std::string> _command;
        user *_user;
        
        command();
        command(std::string message, user *user);
        ~command();

        command &operator=(command const & rhs);

        void execute();
};

std::ostream &operator<<(std::ostream &o, command &rhs);

#endif