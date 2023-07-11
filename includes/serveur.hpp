#ifndef SERVEUR_HPP
# define SERVEUR_HPP

# include <iostream>
# include <sstream>
# include <cstdlib>
# include <unistd.h> 
# include <sys/socket.h>
# include <netinet/in.h>
# include <poll.h>
# include <fcntl.h>
# include <vector>
# include <map>
# include <string.h>
# include "user.hpp"
# include "channel.hpp"
# include "utils.hpp"

# define BUFF_SIZE 1024
// class user;
/*A faire*/

// Fonction d'assignation
// Remttre en prive

class channel;

class user;

class serveur{
    private:
    public:
        std::vector<pollfd> _pollfds;
        std::map<int, user *> _users;
        std::map<std::string, channel *>  _channels;

        int                 _nbUser;
        int			        _port;
        std::string	        _password;
        std::string         _oper;
        int                 _main_socket;
        sockaddr_in         _address;
        std::string        _name;


    /*          Constructeurs && Destructeurs           */

        serveur();
        serveur(int port, std::string password);
        serveur(const serveur & rhs);
        ~serveur();

    /*          Fonctions Membres           */

        int init();
        int loop();
        void addUser();
        int delUser(std::vector<pollfd>::iterator it);

    /*          Operateurs          */

        serveur &operator=(serveur const & rhs);

    /*          Utils           */

        int         getport();
        std::string	getpassword();
        int         getmain_socket();
        sockaddr_in getaddress();

        pollfd      createfd(int fd);

        void        affichage();
        int         erroring(const char *message);
};

#endif
