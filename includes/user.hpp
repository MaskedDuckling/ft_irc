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
#include "replies.hpp"

class command;
class serveur;
class channel;

class user{
	private:
	public:
		std::string             _realname;
		std::string				_username;
		std::string             _nick;
		sockaddr_in             _address;
		std::vector<command *> _commands;
		int                     _fd;
		serveur                 *_serv;
		std::string             _mode;
		std::map<std::string, channel *>  _channels;
		std::vector<std::string> _chan_name;
		std::string             _status;
		std::string				_password;
		std::string				_buffito;
		std::string				_invited;


		user();
		user(int fd, sockaddr_in address, serveur *serv);
		~user();

		void parse_commands(std::string message);
		void execute_commands();
		int isUser(std::string nick);

};

void clear_terminal(int fd);

#endif
