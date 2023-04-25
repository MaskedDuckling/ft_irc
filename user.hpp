#ifndef USER_HPP
# define USER_HPP

#include <iostream>
#include <string.h>

class user{
    private:
        std::string _realname;
        std::string _nick;
        int _fd;
        
    public:
        user();
        user(int fd);
        ~user();
};

#endif