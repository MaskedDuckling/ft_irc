#ifndef CHANNEL_HPP
# define CHANNEL_HPP

#include <vector>
#include <list>
#include <string>
#include "user.hpp"
#include "serveur.hpp"
#include "command.hpp"

class user;
class serveur;
class command;;

class channel{
    private:
    public:
    
        std::string _name;
        std::string _topic;
		std::vector <char> _mode;
		std::string _key;
		int	_limit;
        std::vector <user *> _users;
		std::vector <user *> _operators;
        std::list <std::string> _history;
        serveur	*_serv;

        channel();
        channel(std::string name, user *user, serveur *serv);
        ~channel();

        void broadcast(std::string response, std::string usr);
        void print_msg(std::vector<std::string> str, user *user);
		void print_history(user *user);
        void add_user(user *user, int mode);
        void delete_user(user *user);
		void add_operator(user *user);
		void delete_operator(user *user);
        void list_users();

		void addMode(char c, std::string param);
		void deleteMode(char mode, std::string param);
		int getMode(char c);
		void setLimit(int limit);
		void removeLimit();
		unsigned int getNbUsers();
		void setKey(std::string key);
		void deleteKey();
		int isKeySet();
		int isModeSet(char c);
		int checkOper(std::string user);
};

#endif
