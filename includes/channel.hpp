#ifndef CHANNEL_HPP
# define CHANNEL_HPP

#include <vector>
#include <list>
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
        std::vector <user *> _users;
        std::list <std::string> _history;
        serveur	*_serv;

        channel();
        channel(std::string name, user *user, serveur *serv);
        ~channel();

        void broadcast(std::string response);
        void print_msg(std::vector<std::string> str, user *user);
        void add_user(user *user);
        void delete_user(user *user);
        void list_users();
};

#endif