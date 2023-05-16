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
#include "command.hpp"
#include "serveur.hpp"
#include "channel.hpp"

class command;
class serveur;
class channel;

class user{
	private:
	public:
		std::string             _realname;
		std::string             _nick;
		sockaddr_in             _address;
		std::vector<command *> _commands;
		int                     _fd;
		serveur                 *_serv;
		std::string             _mode;
		channel                 *_channel;
		std::string             _status;
		std::string				_password;


		user();
		user(int fd, sockaddr_in address, serveur *serv);
		~user();

		void parse_commands(std::string message);
		void execute_commands();
		int add_mode(std::string mode);
		int deleteUserMode(char newMode);

};

#endif