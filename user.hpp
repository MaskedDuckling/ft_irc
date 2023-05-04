#ifndef USER_HPP
# define USER_HPP

#include <iostream>
#include <sstream>
#include <cstdlib>
#include <unistd.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <poll.h>
#include <fcntl.h>
#include <vector>
#include <string.h>

class user{
    private:
        std::string _realname;
        std::string _nick;
        sockaddr_in _address;
        int _fd;
        
    public:
        user();
        user(int fd, sockaddr_in address);
        ~user();

        void parse(std::string message);
};

#endif