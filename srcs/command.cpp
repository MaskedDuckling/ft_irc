#include "../includes/command.hpp"

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

std::map<std::string, void (command::*)()> command::_map_fonction;

////////////        	  constructor         //


command::command()
{

}

command::command(std::string message, user *user):_user(user)
{
    while (message.find(" ") != std::string::npos)
    {
        _command.push_back(message.substr(0, message.find(" ")));
        message = message.substr(message.find(" ")+1);
    }
    if (message.size() > 0){
        _command.push_back(message.substr(0, message.find("\n")));
        message = message.substr(message.find("\n")+1);
    }
}



////////////        	  destructor          //



command::~command()
{

}



////////////			public fonctions        //

//				utility fonction			//

void command::execute(){
    init_func_map();
    if (!_command.size()){
        std::cout << "No command to execute" << std::endl;
        return ;
    }
    //std::cout << "Executing command: " << *this << std::endl;

    if (_map_fonction.find(_command[0]) != _map_fonction.end())
            (this->*_map_fonction[_command[0]])();
    else if (_command[0] == "CAP"){}
    else if (_user->_channels.size() > 0)
    {
		std::string str = _user->_chan_name.back();
        for (std::map<std::string, channel *>::iterator _chan = _user->_channels.begin(); _chan != _user->_channels.end(); _chan++)
        {
            if (str == _chan->first)
            {
                _chan->second->print_msg(_command, _user);
                return ;
            }
        }
    }
    else
        std::cout << "Command not found" << std::endl;
}

void command::display_reply(std::string reply, ...){        /*Probablement erreur ici*/
    std::string message;
    va_list arg_list;
    va_start(arg_list,reply);
    for (std::string::iterator it = reply.begin() ; it != reply.end(); it++){
        if (*it!='<')
            message += *it;
        else{
            while (it != reply.end() && *it != '>')
                it++;
            message += va_arg(arg_list, char *);
        }
    }
    message += "\r\n";
    //std::cout << message;
    send(_user->_fd, message.c_str(), message.size(), 0);
    va_end(arg_list);
}

void command::init_func_map()
{
    _map_fonction.insert(std::make_pair("PASS",&command::PASS));
    _map_fonction.insert(std::make_pair("USER",&command::USER));
    _map_fonction.insert(std::make_pair("NICK",&command::NICK));

    _map_fonction.insert(std::make_pair("JOIN",&command::JOIN));
    _map_fonction.insert(std::make_pair("OPER",&command::OPER));
    _map_fonction.insert(std::make_pair("MODE",&command::MODE));


    _map_fonction.insert(std::make_pair("TOPIC",&command::TOPIC));
    _map_fonction.insert(std::make_pair("WHOIS",&command::WHOIS));


    _map_fonction.insert(std::make_pair("PART",&command::PART));
	_map_fonction.insert(std::make_pair("KICK",&command::KICK));

	_map_fonction.insert(std::make_pair("PRIVMSG",&command::PRIVMSG));
    _map_fonction.insert(std::make_pair("INVITE",&command::INVITE));

    _map_fonction.insert(std::make_pair("PING",&command::PING));
    _map_fonction.insert(std::make_pair("PONG",&command::PONG));
}



//				command fonction	for log			//

void command::PASS(){
    if (_command.size() < 2)
        return display_reply(ERR_NEEDMOREPARAMS, "PASS");
    if (_user->_status == "Registered")
        return display_reply(ERR_ALREADYREGISTRED);
    if (_user->_serv->_password != _command[1])
        return display_reply(ERR_PASSWDMISMATCH);
    _user->_status = "Registered";
    _user->_password = _command[1];
    display_reply("\n\033[0;32mPassword correct !\n\033[0m\033[0;33mPlease enter your nickname with \033[0mNICK \"NAME\" \033[0;33m: \033[0m");
}
void command::USER(){
    if (_command.size() < 5)
        return display_reply(ERR_NEEDMOREPARAMS, "USER");
    if (_user->_status != "Registered")
        return display_reply(ERR_ALREADYREGISTRED);
    _user->_realname = _command[1];
    if (_user->_nick != ""){
        std::string tmp = inet_ntoa(_user->_address.sin_addr);
        display_reply(CLEAR_TERM);
        display_reply(RPL_WELCOME, _user->_nick.c_str(), _user->_realname.c_str(), tmp.c_str());
		display_reply(RPL_YOURHOST, _user->_serv->_name.c_str(), "1.0");
		display_reply(RPL_CREATED, "today");
		display_reply(RPL_MYINFO, _user->_serv->_name.c_str(), "1.0", "*", "*");
        _user->_status = "Connected";
    }
}
void command::NICK(){
    if (_command.size() < 2)
        return display_reply(ERR_NONICKNAMEGIVEN);
    if (_user->_mode.find('r') != std::string::npos)
        return display_reply(ERR_RESTRICTED);
    for (std::map<int, user *>::iterator it = _user->_serv->_users.begin(); it != _user->_serv->_users.end(); it++)
        if (it->second->_nick == _command[1])
            return display_reply(ERR_NICKNAMEINUSE, _command[1].c_str());
    // if (kekchose)
    //     return display_reply(ERR_ERRONEUSNICKNAME, _command[1].c_str());
    // if (kekchose d'autre)
    //     return display_reply(ERR_UNAVAILRESOURCE, _command[1].c_str());
    _user->_nick = _command[1];
    display_reply("\n\033[0;32mNickname register !\n\033[0m\033[0;33mPlease enter your username with \033[0mUSER \"username\" \"mode\" \"unused\" \"realname\" \033[0;33m: \033[0m");
    if (_user->_realname != ""){
        std::string tmp = inet_ntoa(_user->_address.sin_addr);
        display_reply(CLEAR_TERM);
        display_reply(RPL_WELCOME, _user->_nick.c_str(), _user->_realname.c_str(), tmp.c_str());
		display_reply(RPL_YOURHOST, _user->_serv->_name.c_str(), "1.0");
		display_reply(RPL_CREATED, "today");
		display_reply(RPL_MYINFO, _user->_serv->_name.c_str(), "1.0", "*", "*");
        _user->_status = "Connected";
    }
}



//				command fonction			//


void command::JOIN()
{
    if (_command.size() < 2)
        return display_reply(ERR_NEEDMOREPARAMS, _command[0].c_str());                                                                                                            //A peut etre modifier
    for (std::map<std::string, channel *>::iterator it = _user->_serv->_channels.begin(); it != _user->_serv->_channels.end(); it++)
    {
        if (it->first == _command[1])
        {
            for (std::vector<char>::iterator it2 = it->second->_mode.begin(); it2 != it->second->_mode.end(); it2++)
			{
				if (*it2 == 'i')
				{
					display_reply(ERR_INVITEONLYCHAN, _command[1].c_str());
					return ;
				}
				else if (it->second->_limit > -1 && it->second->_limit <= (int)it->second->_users.size())
				{
					display_reply(ERR_CHANNELISFULL, _command[1].c_str());
					return ;
				}
				else if (*it2 == 'k')
				{
					if (_command.size() < 3)
					{
						display_reply(ERR_PASSWDMISMATCH);
						return ;
					}
					else if (it->second->_key != _command[2])
					{
						display_reply(ERR_PASSWDMISMATCH);
						return ;
					}
				}
			}
            if (_user->_channels.size() > 0)
            {
                for (std::vector <user *>::iterator it2 = it->second->_users.begin(); it2 != it->second->_users.end(); it2++)
                {
                    if ((*it2)->_nick == _user->_nick)
                    {
                        display_reply(CLEAR_TERM);
                        it->second->print_history(_user);
                        for (std::vector <std::string>::iterator chan = _user->_chan_name.begin(); chan != _user->_chan_name.end(); chan++)
                        {
                            if (*chan == it->first)
                            {
                                _user->_chan_name.push_back(it->first);
                                std::string str = "\033[0;33mRecover chat from " + it->first + "\033[0m\n";
                                send((*it2)->_fd, str.c_str(), str.size(), 0);
                                return ;
                            }
                        }
                        return ;
                    }
                }
            }
            display_reply(CLEAR_TERM);
            it->second->add_user(_user, 1);
            return ;
        }
    }
    display_reply(CLEAR_TERM);
    _user->_serv->_channels[_command[1]] = new channel(_command[1], _user, _user->_serv);
}


void command::OPER()
{
    if (_command.size() < 3)
	{
        return display_reply(ERR_NEEDMOREPARAMS, _command[0].c_str());
	}
    if (_command[1] != _user->_nick)
	{
        return display_reply(ERR_ERRONEUSNICKNAME, _command[1].c_str());
	}
    if (_command[2] != _user->_serv->_oper)
	{
        return display_reply(ERR_PASSWDMISMATCH);
	}
	if (_user->_mode.find('o') != std::string::npos)
	{
		display_reply("You are already an operator");
		return ;
	}
    if (!_user->add_mode("o", 1))
        return display_reply(ERR_NOOPERHOST);
    return display_reply(RPL_YOUREOPER);
}

/**
 * @brief Pour le mode o (operateur) ajouter la necessiter d'etre OPER pour l'utiliser
 * et check que l'user target de o existe
 * 
 */

void command::MODE()
{	
	std::string modes = "itkol";
	int	j = 0;
	int y = 0;

	if (_command.size() < 3)
	{
		display_reply(ERR_NEEDMOREPARAMS, _command[0].c_str());
		return ;
	}
	for (std::map<std::string, channel *>::iterator it = _user->_serv->_channels.begin(); it != _user->_serv->_channels.end(); it++)
	{
		if (it->first == _command[1])
		{
			if (it->second->checkOper(_user->_nick) == 0)
			{
				if (_user->_mode.find('o') == std::string::npos)
				{
					display_reply(ERR_CHANOPRIVSNEEDED, _command[1].c_str());
					return ;
				}
			}
			if (_command[2][0] == '+')
			{
				_command[2].erase(0, 1);
				while (_command[2].size() > 0)
				{
					j = 0;
					for (int i = 0; i < (int)modes.size(); i++)
					{
						if (_command[2][0] == modes[i])
						{
							j = 1;
							if ((_command[2][0] == 'k') && (it->second->isKeySet() == 0))
							{
								display_reply(ERR_KEYSET, _command[1].c_str());
								return ;
							}
							if (_command[2][0] == 'o' && _command.size() > 3)
							{
								y = 0;
								for (std::vector<user*>::iterator it2 = it->second->_users.begin() ; it2 != it->second->_users.end() ; it2++)
								{
									if ((*it2)->_nick == _command[3])
										y = 1;
								}
								if (y == 0)
								{
									display_reply(ERR_USERNOTINCHANNEL, _command[3].c_str(), _command[1].c_str());
									return ;
								}
							}
							if (it->second->isModeSet(_command[2][0]) == 1 && _command[2][0] != 'l')
							{
								
							}
							else if (_command[2][0] == 'k' || _command[2][0] == 'l' || _command[2][0] == 'o')
							{
								if (_command.size() < 4)
								{
									display_reply(ERR_NEEDMOREPARAMS, _command[0].c_str());
									return ;
								}
								it->second->addMode(_command[2][0], _command[3]);
								_command.erase(_command.begin() + 3);
							}
							else
							{
								it->second->addMode(_command[2][0], "");
							}
						}
					}
					if (j == 0)
						display_reply(ERR_UNKNOWNMODE, _command[2][0], _command[1].c_str());
					_command[2].erase(0, 1);
				}
			}
			else if (_command[2][0] == '-')
			{
				_command[2].erase(0, 1);
				while (_command[2].size() > 0)
				{
					j = 0;
					for (int i = 0; i < (int)modes.size(); i++)
					{
						if (_command[2][0] == modes[i])
						{
							j = 1;
							if (_command[2][0] == 'o')
							{
								if (_command.size() < 4)
								{
									display_reply(ERR_NEEDMOREPARAMS, _command[0].c_str());
									return ;
								}
								y = 0;
								for (std::vector<user*>::iterator it2 = it->second->_users.begin(); it2 != it->second->_users.end(); it2++)
								{
									if ((*it2)->_nick == _command[3])
									{
										y = 1;
									}
								}
								if (y == 0)
								{
									display_reply(ERR_USERNOTINCHANNEL, _command[3].c_str(), _command[1].c_str());
									return ;
								}
								it->second->deleteMode(_command[2][0], _command[3]);
								_command.erase(_command.begin() + 3);
							}
							else
								it->second->deleteMode(_command[2][0], "");
						}
					}
					if (j == 0)
						display_reply(ERR_UNKNOWNMODE, _command[2][0], _command[1].c_str());
					_command[2].erase(0, 1);
				}
			}
			return ;
		}
	}
	display_reply(ERR_NOSUCHCHANNEL, _command[1].c_str());
	return ;
}




void command::TOPIC()
{
    if (_command.size() < 2)
        return display_reply(ERR_NEEDMOREPARAMS, "TOPIC");
    unsigned long i = 0;
    std::map<std::string, channel *>::iterator it = _user->_serv->_channels.begin();
    while (it != _user->_serv->_channels.end() && it->second->_name != _command[1])
    {
        it++;
        i++;
    }
    if (it == _user->_serv->_channels.end())
	{
        return display_reply("\033[0;31mChannel not created\033[0m");
	}
    if (i >= _user->_serv->_channels.size())
	{
        return display_reply(ERR_NOSUCHNICK);
	}
    std::map<std::string, channel *>::iterator it3 = _user->_channels.begin();
    i = 0;
    while (it3 != _user->_channels.end() && it3->second->_name != _command[1])
    {
        it3++;
        i++;
    }
    if (it3 == _user->_channels.end())
        return display_reply("\033[0;31mNo channel joined\033[0m");
    if (i >= _user->_channels.size())
        return display_reply(ERR_NOTONCHANNEL, _command[1].c_str());
    if (_command.size() == 2)
    {
        for (std::map<std::string, channel *>::iterator it2 = _user->_serv->_channels.begin(); it2 != _user->_serv->_channels.end(); it2++)
        {
            if (it2->second->_name == _command[1])
            {
                send(_user->_fd, it2->second->_topic.c_str(), it2->second->_topic.size(), 0);
                return ;
            }
        }
    }
    else if (_command.size() > 2)
    {
        for (std::map<std::string, channel *>::iterator it2 = _user->_serv->_channels.begin(); it2 != _user->_serv->_channels.end(); it2++)
        {
            if (it2->second->_name == _command[1])
            {
				for (std::vector <char>::iterator cha = it2->second->_mode.begin(); cha != it2->second->_mode.end(); cha++)
				{
					if (*cha == 't')
					{
                        std::vector <user *>::iterator user = it2->second->_users.begin();
						while (user != it2->second->_users.end() && (*user)->_nick != _user->_nick)
                            user++;
                        if (user == it2->second->_users.end())
                            return display_reply("\033[0;31mNo channel joined\033[0m");
                        if (it->second->checkOper(_user->_nick) == 0)
			            {
				            if (_user->_mode.find('o') == std::string::npos)
				            {
					            display_reply(ERR_CHANOPRIVSNEEDED, _command[1].c_str());
					            return ;
				            }
		            	} 
					}
				}
				std::string str = "\033[0;36mChannel topic: ";
                if (_command[2] == ":")
                {
                    it2->second->_topic.clear();
                    str = "\033[0;34mTopic has been clean for channel " + it2->second->_name +  " by " + _user->_nick + "\n\033[0m";
                    it2->second->broadcast(str, "-1");
                    return ;
                }
                unsigned long i = 2;
                while (i < _command.size())
                {
                    str += _command[i++];
                    str += " ";
                }
                it->second->_topic = str + "\033[0m\n";
                str = "\033[0;34mTopic has been set for channel " + it2->second->_name +  " by " + _user->_nick + "\n\033[0m";
                it2->second->broadcast(str, "-1");
                return ;
            }
        }
    }
}

void command::WHOIS(){
    if (_command.size() < 2)
        return display_reply(ERR_NONICKNAMEGIVEN);
    for (std::map<int, user *>::iterator it = _user->_serv->_users.begin(); it != _user->_serv->_users.end(); it++)
        if (it->second->_nick == _command[1])
                return display_reply(RPL_WHOISUSER, _user->_nick.c_str(), _user->_realname.c_str(), _user->_serv->_name.c_str(), _user->_realname.c_str());
    return display_reply(ERR_NOSUCHNICK, _command[1].c_str());
}





void command::PART()
{
    if (_command.size() < 2)
        return display_reply(ERR_NEEDMOREPARAMS, _command[0].c_str());
    for (std::map<std::string, channel *>::iterator it = _user->_serv->_channels.begin(); it != _user->_serv->_channels.end(); it++)
    {
        if (it->first == _command[1])
        {
            for (std::map<std::string, channel *>::iterator at = _user->_channels.begin(); at != _user->_channels.end(); at++)
            {
                if (at->first == _command[1])
                {
                    std::string str = "\033[0;34m";
                    str += _user->_nick;
                    str += " has left the channel\033[0m\n";
                    at->second->broadcast(str, "-1");
                    at->second->delete_user(_user);
                    _user->_channels.erase(at);
					return ;
                }
            }
        }
    }
}

void command::KICK()
{
	if (_command.size() < 3)
		return display_reply(ERR_NEEDMOREPARAMS, _command[0].c_str());
	if (_command.size() == 3)	 /*	KICK "channel" "USER"	*/
	{
		if (_command[1][0] == '#' || _command[1][0] == '&')
			_command[1].erase(0, 1);
		for (std::map<int, user*>::iterator it = _user->_serv->_users.begin(); it != _user->_serv->_users.end(); it++)
		{
			if (it->second->_nick == _command[2])
			{
				for (std::map<std::string, channel *>::iterator it2 = it->second->_channels.begin(); it2 != it->second->_channels.end(); it2++)
				{
					if (it2->first == _command[1])
					{
						std::string str = "\033[0;34m";
						str += it->second->_nick;
						str += " has been kicked from the channel ";
						str += it2->first;
						str += "\033[0m\n";
						it2->second->broadcast(str, "-1");			/*	Notifie les users du channel du kick	*/
						it2->second->delete_user(it->second); /*	Supprime l'user de la liste du channel	*/
						it->second->_channels.erase(it2);		/*	Supprime le channel de la liste de l'user	*/
						return;
					}
				}
				return display_reply(ERR_USERNOTINCHANNEL, _command[2].c_str(), _command[1].c_str());
			}
		}
		return display_reply(ERR_NOSUCHNICK, _command[2].c_str());
	}
	else if (_command.size() > 3 && (_command[3].c_str()[0] == ':'))
	{
		if (_command[1][0] == '#' || _command[1][0] == '&')
			_command[1].erase(0, 1);
		std::string com = merge(_command, 4);
		for (std::map<int, user*>::iterator it = _user->_serv->_users.begin(); it != _user->_serv->_users.end(); it++)
		{
			if (it->second->_nick == _command[2])
			{
				for (std::map<std::string, channel *>::iterator it2 = it->second->_channels.begin(); it2 != it->second->_channels.end(); it2++)
				{
					if (it2->first == _command[1])
					{
						std::string str = "\033[0;34m";
						str += it->second->_nick;
						str += " has been kicked from the channel ";
						str += it2->first;
						str += " cause : ";
						str += com;
						str += "\033[0m\n";
						it2->second->broadcast(str, "-1");			/*	Notifie les users du channel du kick	*/
						it2->second->delete_user(it->second); /*	Supprime l'user de la liste du channel	*/
						it->second->_channels.erase(it2);		/*	Supprime le channel de la liste de l'user	*/
						return;
					}
				}
				return display_reply(ERR_USERNOTINCHANNEL, _command[2].c_str(), _command[1].c_str());
			}
		}
		return display_reply(ERR_NOSUCHNICK, _command[2].c_str());
	}
	else if (_command.size() > 3)
	{
		if (_command[2][0] == '#' || _command[2][0] == '&')
		{
			unsigned long int i = 1;
			int chanN = 0;
			int userN = 0;
			while (i < _command.size())
			{
				if (_command[i][0] == '#' || _command[i][0] == '&')
					chanN++;
				else
					break;
				_command[i].erase(0, 1);
				i++;
			}
			while (i < _command.size())
			{
				userN++;
				i++;
			}
			if (chanN != userN)
				return display_reply(ERR_NEEDMOREPARAMS, _command[0].c_str());
			while (chanN > 0)
			{
				int b = 0;
				for (std::map<int, user*>::iterator it = _user->_serv->_users.begin(); it != _user->_serv->_users.end(); it++)
				{
					if (it->second->_nick == _command[1 + chanN])
					{
						for (std::map<std::string, channel *>::iterator it2 = it->second->_channels.begin(); it2 != it->second->_channels.end(); it2++)
						{
							if (it2->first == _command[1])
							{
								it2->second->delete_user(it->second);	/*	Supprime l'user de la liste du channel		*/
								it->second->_channels.erase(it2);		/*	Supprime le channel de la liste de l'user	*/
								b = 1;
								break ;
							}
						}
						if (b == 0)
						{
							b = 2;
							display_reply(ERR_USERNOTINCHANNEL, _command[1 + chanN].c_str(), _command[1].c_str());
						}
						break ;
					}
				}
				if (b == 2)
					display_reply(ERR_NOSUCHNICK, _command[1 + chanN].c_str());
				_command.erase(_command.begin() + 1);
				if (_command.size() >= 2)
					_command.erase(_command.begin() + ((_command.size() - 1) - (chanN - 1)));
				chanN--;
			}
		}
		else
		{
			unsigned long int i = 2;
			int userN = 0;
			while (i < _command.size())
			{
				userN++;
				i++;
			}
			while (userN > 0)
			{
				int b = 0;
				for (std::map<int, user*>::iterator it = _user->_serv->_users.begin(); it != _user->_serv->_users.end(); it++)
				{
					if (it->second->_nick == _command[2])
					{
						for (std::map<std::string, channel *>::iterator it2 = it->second->_channels.begin(); it2 != it->second->_channels.end(); it2++)
						{
							if (it2->first == _command[1])
							{
								it2->second->delete_user(it->second);	/*	Supprime l'user de la liste du channel		*/
								it->second->_channels.erase(it2);		/*	Supprime le channel de la liste de l'user	*/
								b = 1;
								break ;
							}
						}
						if (b == 0)
						{
							b = 2;
							display_reply(ERR_USERNOTINCHANNEL, _command[2].c_str(), _command[1].c_str());
						}
						break ;
					}
				}
				if (b == 2)
					display_reply(ERR_NOSUCHNICK, _command[2].c_str());
				_command.erase(_command.begin() + 2);
				userN--;
			}
		}
	}
	return ;
}

void command::INVITE()
{
    if (_command.size() < 3)
        return display_reply(ERR_NEEDMOREPARAMS, _command[0].c_str());
    if (_user->_mode.find('o') == std::string::npos)
        return display_reply("Need to be operator tu use this command");
    std::map<int, user *>::iterator it = _user->_serv->_users.begin();
    if (it == _user->_serv->_users.end())
    {
        return ;
    }
	while (it != _user->_serv->_users.end())
    {
        if (it->second->_nick == _command[1])
        {
            break;
        }
        it++;
    }
    if (it == _user->_serv->_users.end())
    {
        return display_reply(ERR_NOSUCHNICK, _command[1].c_str());
    }
    std::map<std::string, channel *>::iterator it2 = _user->_serv->_channels.begin();
    if (it2 == _user->_serv->_channels.end())
    {
        return ;
    }
    while (it2 != _user->_serv->_channels.end())
    {
        if ( it2->first == _command[2])
        {
            break;
        }
        it2++;
    }
    if (it2 == _user->_serv->_channels.end())
    {
        return ;
    }
    std::vector <user *>::iterator _use = it2->second->_users.begin();
    while (_use != it2->second->_users.end())
    {
        if ((*_use)->_nick == _user->_nick)
        {
            break;
        }
        _use++;
    }
    if (_use == it2->second->_users.end())
	    return display_reply(ERR_NOTONCHANNEL, _command[2].c_str());
    _use = it2->second->_users.begin();
    while (_use != it2->second->_users.end())
    {
        if ((*_use)->_nick == _command[2])
        {
            break;
        }
        _use++;
    }
    if (_use != it2->second->_users.end())
	    return display_reply(ERR_USERONCHANNEL, _command[1].c_str(), _command[2].c_str());
    it2->second->add_user(it->second, 0);
    std::string str = "\x1B[2J\x1B[H";
    send(it->second->_fd, str.c_str(), str.size(), 0);
    it2->second->print_history(it->second);
    str = "\033[0;34mYou have been invite to join the channel " + it2->first + "\033[0m\n";
    send(it->second->_fd, str.c_str(), str.size(), 0);
    str = "\033[0;34mYou invite " + it->second->_nick + " to join the channel " + it2->first + "\033[0m\n";
    send(_user->_fd, str.c_str(), str.size(), 0);
}

void command::PRIVMSG()
{
    if (_command.size() < 3)
        return display_reply(ERR_NEEDMOREPARAMS, _command[0].c_str());
    for (std::map<int, user *>::iterator it = _user->_serv->_users.begin(); it != _user->_serv->_users.end(); it++)
    {
        if (it->second->_nick == _command[1])
        {
            std::string str;
            if (it->second->_mode.find("a") != std::string::npos)
            {
                str = "\033[0;31mUser " + it->second->_nick + " is away\033[0m\n";
                send(it->second->_fd, str.c_str(), str.size(), 0);
                return ;
            }
            str = "\033[0;35m";
            unsigned long i = 2;
            str += _user->_nick;
            str += " : ";
            while (i < _command.size())
            {
                str += _command[i++];
                str += " ";
            }
            str += "\033[0m\n";
            send(it->second->_fd, str.c_str(), str.size(), 0);
            return ;
        }
    }
    for (std::map<std::string, channel *>::iterator it = _user->_serv->_channels.begin(); it != _user->_serv->_channels.end(); it++)
    {
        if (it->first == _command[1])
        {
            std::string str = "\033[0;34m#" + it->first + " ";
            unsigned long i = 2;
            str += _user->_nick;
            str += " : ";
            while (i < _command.size())
            {
                str += _command[i++];
                str += " ";
            }
            str += "\033[0m\n";  
            it->second->broadcast(str, _user->_nick);
            return ;
        }
    }
}


void command::PING(){
    std::string reply = "PONG : ";
    if (_command.size() < 2)
        return display_reply(ERR_NOORIGIN);
    reply += _user->_serv->_name.c_str();
    return display_reply(reply);
}
void command::PONG(){
    if (_command.size() < 2)
        return display_reply(ERR_NOORIGIN);
}



std::ostream &operator<<(std::ostream &o, command &rhs){
    for(std::vector<std::string>::iterator it = rhs._command.begin(); it != rhs._command.end(); it++){
        o << *it;
        if (it != rhs._command.end()-1)
            std::cout << " ";
    }
    return (o);
}
