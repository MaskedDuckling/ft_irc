#ifndef COMMAND_HPP
# define COMMAND_HPP

#include <vector>
#include <iostream>
#include <string>
#include <fstream>
#include <signal.h>
#include <map>
#include <sys/types.h>
#include <sys/socket.h>
#include "replies.hpp"
#include "user.hpp"
#include "utils.hpp"

class user;

class command{
    static std::map<std::string, void (command::*)()> _map_fonction;
    // private:
    public:
        std::vector<std::string> _command;
        user *_user;
        

        command();
        command(std::string message, user *user);
        ~command();

        command &operator=(command const & rhs);
        
        static void init_func_map();

        void PASS();
        void USER();
        void NICK();

        void JOIN();
        void OPER();
        void MODE();
        void TOPIC();
        void WHOIS();

        void PART();
        void QUIT();

        void PING();
        void PONG();

		void KICK();
        void PRIVMSG();

        void display_reply(std::string reply,...);
        void execute();

        class UserQuitServer : public std::exception
        {	
	        public:
		
		        const char* what() const throw();
        };
};

std::ostream &operator<<(std::ostream &o, command &rhs);

#endif
