#include "../includes/command.hpp"

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <stdarg.h>

std::map<std::string, void (command::*)()> command::_map_fonction;

////////////        	  constructor         //

command::command()
{
}

command::command(std::string message, user *user) : _user(user)
{
	while (message.find(" ") != std::string::npos)
	{
		_command.push_back(message.substr(0, message.find(" ")));
		message = message.substr(message.find(" ") + 1);
	}
	if (message.size() > 0)
	{
		_command.push_back(message.substr(0, message.find("\n")));
		message = message.substr(message.find("\n") + 1);
	}
}

////////////        	  destructor          //

command::~command()
{
}

////////////			public fonctions        //

//				utility fonction			//

void command::execute()
{
	if (!_command.size())
	{
		std::cout << "No command to execute" << std::endl;
		return;
	}
	std::cout << "Executing command: " << *this << std::endl;

	if (_map_fonction.find(_command[0]) != _map_fonction.end())
		(this->*_map_fonction[_command[0]])();
	else if (_command[0] == "CAP")
	{
	}
	// else if (_user->_channels.size() > 0){}
	else
		std::cout << "Command not found" << std::endl;
}

void command::display_reply(std::string msg, ...)
{
	va_list vl;
	std::string dest_nick = _user->_nick;

	if (dest_nick == "")
		dest_nick = "*";
	std::string message(":" + _user->_serv->_name + " " + msg.substr(0, 4) + dest_nick + " ");
	size_t i = 4;

	va_start(vl, msg);
	while (i < msg.length())
	{
		if (msg[i] != '<')
			message += msg[i];
		else
		{
			while (msg[i] != '>')
				i++;
			message += va_arg(vl, char *);
		}
		i++;
	}
	message += "\n";
	std::cout << message << std::endl;
	send(_user->_fd, message.c_str(), message.size(), 0);
	va_end(vl);
}

void command::init_func_map()
{
	_map_fonction.insert(std::make_pair("PASS", &command::PASS));
	_map_fonction.insert(std::make_pair("USER", &command::USER));
	_map_fonction.insert(std::make_pair("NICK", &command::NICK));

	_map_fonction.insert(std::make_pair("JOIN", &command::JOIN));
	_map_fonction.insert(std::make_pair("OPER", &command::OPER));
	_map_fonction.insert(std::make_pair("MODE", &command::MODE));

	_map_fonction.insert(std::make_pair("TOPIC", &command::TOPIC));
	_map_fonction.insert(std::make_pair("WHOIS", &command::WHOIS));

	_map_fonction.insert(std::make_pair("PART", &command::PART));
	_map_fonction.insert(std::make_pair("KICK", &command::KICK));

	_map_fonction.insert(std::make_pair("PRIVMSG", &command::PRIVMSG));
	_map_fonction.insert(std::make_pair("INVITE", &command::INVITE));

	_map_fonction.insert(std::make_pair("PING", &command::PING));
	_map_fonction.insert(std::make_pair("PONG", &command::PONG));

	_map_fonction.insert(std::make_pair("QUIT", &command::QUIT));
}

//				command fonction	for log			//

void command::PASS()
{
	if (_command.size() < 2)
		return display_reply(ERR_NEEDMOREPARAMS, "PASS");
	if (_user->_status == "Registered")
		return display_reply(ERR_ALREADYREGISTRED);
	if (_user->_serv->_password != _command[1])
		return display_reply(ERR_PASSWDMISMATCH);
	_user->_status = "Registered";
	_user->_password = _command[1];
}
void command::USER()
{
	if (_command.size() < 5)
		return display_reply(ERR_NEEDMOREPARAMS, "USER");
	if (_user->_status != "Registered")
		return display_reply(ERR_ALREADYREGISTRED);
	_user->_realname = _command[1];
	if (_user->_nick != "" && _user->_status != "Connected")
	{
		std::string tmp = inet_ntoa(_user->_address.sin_addr);
		display_reply(RPL_WELCOME, _user->_nick.c_str(), _user->_realname.c_str(), tmp.c_str());
		display_reply(RPL_YOURHOST, _user->_serv->_name.c_str(), "1.0");
		display_reply(RPL_CREATED, "today");
		display_reply(RPL_MYINFO, _user->_serv->_name.c_str(), "1.0", "*", "*");
		_user->_status = "Connected";
	}
}
void command::NICK()
{
	if (_command.size() < 2)
		return display_reply(ERR_NONICKNAMEGIVEN);
	if (_user->_mode.find('r') != std::string::npos)
		return display_reply(ERR_RESTRICTED);
	for (std::map<int, user *>::iterator it = _user->_serv->_users.begin(); it != _user->_serv->_users.end(); it++)
		if (it->second->_nick == _command[1])
			return display_reply(ERR_NICKNAMEINUSE, _command[1].c_str());
	std::string oldnick = _user->_nick;
	_user->_nick = _command[1];
	if (_user->_realname != "" && _user->_status != "Connected")
	{
		std::string tmp = inet_ntoa(_user->_address.sin_addr);
		display_reply(RPL_WELCOME, _user->_nick.c_str(), _user->_realname.c_str(), tmp.c_str());
		display_reply(RPL_YOURHOST, _user->_serv->_name.c_str(), "1.0");
		display_reply(RPL_CREATED, "today");
		display_reply(RPL_MYINFO, _user->_serv->_name.c_str(), "1.0", "*", "*");
		_user->_status = "Connected";
	}
	else if (_user->_status == "Connected")
	{
		std::string reply = ":" + oldnick + " NICK " + _command[1] + "\r\n";
		std::cout << reply << std::endl;
		send(_user->_fd, reply.c_str(), reply.size(), 0);
	}
}

//				command fonction			//

void command::JOIN()
{
	if (_command.size() < 2)
		return display_reply(ERR_NEEDMOREPARAMS, _command[0].c_str()); // A peut etre modifier
	for (std::map<std::string, channel *>::iterator it = _user->_serv->_channels.begin(); it != _user->_serv->_channels.end(); it++)
	{
		if (it->first == _command[1])
		{
			if (it->second->getMode('i') == 1 && _user->_invited != _command[1])
			{
				display_reply(ERR_INVITEONLYCHAN, _command[1].c_str());
				return;
			}
			if (it->second->_limit > -1 && it->second->_limit <= (int)it->second->_users.size())
			{
				display_reply(ERR_CHANNELISFULL, _command[1].c_str());
				return;
			}
			if (it->second->getMode('k') == 1)
			{
				std::cout << "key : " << it->second->_key << std::endl;
				if (_command.size() < 3)
				{
					display_reply(ERR_PASSWDMISMATCH);
					return;
				}
				if (it->second->_key != _command[2])
				{
					display_reply(ERR_PASSWDMISMATCH);
					return;
				}
			}
			it->second->add_user(_user, 1);
			// return;
			return join_reply();
		}
	}
	_user->_serv->_channels[_command[1]] = new channel(_command[1], _user, _user->_serv);
	return join_reply();
}

void command::join_reply()
{
	display_reply(RPL_TOPIC, _command[1].c_str(), _user->_channels[_command[1]]->_topic.c_str());
}

void command::PART()
{
	if (_command.size() < 2)
	{
		return display_reply(ERR_NEEDMOREPARAMS, _command[0].c_str());
	}
	for (std::map<std::string, channel *>::iterator it = _user->_channels.begin(); it != _user->_channels.end(); it++)
	{
		if (it->first == _command[1])
		{
			std::string str = ":" + _user->_nick + "!" + _user->_serv->_name + "@localhost" + " PART " + _command[1];
			unsigned long i = 2;
			if (i < _command.size())
				str += " ";
			while (i < _command.size())
			{
				str += _command[i++];
				if (i < _command.size())
					str += " ";
			}
			str += "\r\n";
			it->second->broadcast(str, "");

			it->second->delete_user(_user->_nick);
			_user->_channels.erase(it);
			return;
		}
	}
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
		return;
	}
	if (!add_mode("o"))
		return display_reply(ERR_NOOPERHOST);
	return display_reply(RPL_YOUREOPER);
}

void	command::MODE()
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
		display_reply(ERR_NOSUCHCHANNEL, _command[1].c_str());
	return ;
	}
}

/*void command::MODE()
{
	std::string modes = "itkol";
	int i = 0;

	if (_command.size() < 3)
		return display_reply(ERR_NEEDMOREPARAMS, _command[0].c_str());
	if (_user->_mode.find('o') == std::string::npos)
		return display_reply(ERR_CHANOPRIVSNEEDED, _command[1].c_str());
	if (_command[1] == _user->_nick)
	{
		char c = _command[2][0];
		_command[2].erase(0, 1);
		if (c == '+')
			add_mode(_command[2].c_str());
		else if (c == '-')
			deleteUserMode(_command[2].c_str()[0]);
		else
			return display_reply(ERR_UMODEUNKNOWNFLAG);
		return;
	}
	else if (_user->isUser(_command[1]) == 1)
		return display_reply(ERR_USERSDONTMATCH);
	else{
		for (std::map<std::string, channel *>::iterator it =_user->_channels.begin(); it != _user->_channels.end() ;it++){
			if (it->second->_name == _command[1]){
				i = it->second->change_channel_mode(_command[2][0], _command[2][1], _command);
				// std::cout << it->second->_mode << std::endl;
				// std::string str = it->second->_mode;
				if (i == 1)
					return display_reply(RPL_CHANNELMODEIS, _command[1].c_str(),  it->second->_mode.c_str(), "");
				break;	
			}
		}
		if (i == -1)
			return display_reply(ERR_UNKNOWNMODE, &_command[2][1], _command[1].c_str());
	}
}

*/int command::add_mode(std::string mode){
    std::string available_modes("aiwroOs");
    if (available_modes.find(mode) == std::string::npos)
        return 0;
    else if (_user->_mode.find(mode) == std::string::npos)
    {
        std::string str;
        if (mode == "o" && mode == "O" && mode == "a")
			return 2;
        else
        {
            _user->_mode += mode;
			display_reply(RPL_UMODEIS, _user->_mode.c_str());
		}
    }
    return 1;
}
int command::deleteUserMode(char newMode){
	size_t n;

	n = _user->_mode.find(newMode);
	if (n != std::string::npos)
    {
		_user->_mode.erase(n, 1);
        std::string str;
        if (newMode == 'a')
            return 2;
        else
			display_reply(RPL_UMODEIS, _user->_mode.c_str());
        return (0);
    }
    return (1);
}

void command::TOPIC()
{
	if (_command.size() < 2)
	{
		return display_reply(ERR_NEEDMOREPARAMS, "TOPIC");
	}
	if (_user->_mode.find('o') == std::string::npos && _user->_channels[_command[1]]->getMode('t') == 1)
		return display_reply(ERR_CHANOPRIVSNEEDED, _command[1].c_str());
	for (std::map<std::string, channel *>::iterator it = _user->_channels.begin(); it != _user->_channels.end(); it++)
	{
		if (it->second->getMode('t') && _user->_mode.find("o") == std::string::npos)
			return display_reply(ERR_CHANOPRIVSNEEDED, _command[1].c_str());
		if (_command[1] == it->second->_name)
		{
			if (_command[2] == "" && it->second->_topic == "")
				return display_reply(RPL_NOTOPIC, _command[1].c_str());
			else if (_command[2] == "" && it->second->_topic != "")
				display_reply(RPL_TOPIC, _command[1].c_str(), it->second->_topic.c_str());
			else if (_command[2] == ":" && _command.size() == 3)
			{
				it->second->_topic = "";
				display_reply(RPL_TOPIC, _command[1].c_str(), it->second->_topic.c_str());
			}
			else if (_command.size() >= 3)
			{
				display_topic();
				display_reply(RPL_TOPIC, _command[1].c_str(), it->second->_topic.c_str());
			}
			return;
		}
	}
	return display_reply(ERR_NOTONCHANNEL, _command[1].c_str());
}

void command::display_topic()
{
	_command[2].erase(0, 1);
	std::string reply = ":" + _user->_nick + "!" + _user->_serv->_name + "@localhost" + " TOPIC " + _command[1];
	std::string str = "";
	for (unsigned long i = 2; i < _command.size(); i++)
	{
		str += _command[i];
		if (i < _command.size() - 1)
			str += " ";
	}
	reply += str + "\r\n";
	_user->_channels[_command[1]]->_topic = str;
	_user->_channels[_command[1]]->broadcast(reply, "");
}

void command::WHOIS()
{
	if (_command.size() < 2)
		return display_reply(ERR_NONICKNAMEGIVEN);
	for (std::map<int, user *>::iterator it = _user->_serv->_users.begin(); it != _user->_serv->_users.end(); it++)
		if (it->second->_nick == _command[1])
			return display_reply(RPL_WHOISUSER, _user->_nick.c_str(), _user->_realname.c_str(), _user->_serv->_name.c_str(), _user->_realname.c_str());
	return display_reply(ERR_NOSUCHNICK, _command[1].c_str());
}

void command::KICK()
{
	if (_command.size() < 3)
		return display_reply(ERR_NEEDMOREPARAMS, _command[0].c_str());
	if (_user->_mode.find('o') == std::string::npos)
		return display_reply(ERR_CHANOPRIVSNEEDED, _command[1].c_str());
	std::string str_channel = ":" + _user->_nick + "!" + _user->_serv->_name + "@localhost" + " KICK " + _command[1] + " " + _command[2] + " ";
	std::string str_user = ":" + _user->_nick + "!" + _user->_serv->_name + "@localhost" + " PART " + _command[1] + "\r\n";
	/*	KICK "channel" "USER"	*/
	for (std::map<int, user *>::iterator it = _user->_serv->_users.begin(); it != _user->_serv->_users.end(); it++)
	{
		if (it->second->_nick == _command[2])
		{
			for (std::map<std::string, channel *>::iterator it2 = it->second->_channels.begin(); it2 != it->second->_channels.end(); it2++)
			{
				if (it2->first == _command[1])
				{
					for (unsigned long i = 3; i < _command.size(); i++)
					{
						str_channel += _command[i];
						if (i < _command.size() - 1)
							str_channel += " ";
					}
					str_channel += "\r\n";
					it2->second->broadcast(str_channel, _command[2]);
					std::cout << str_user << std::endl;
					send(it->second->_fd, str_user.c_str(), str_user.size(), 0); /*	Notifie l'user du kick	*/


					it2->second->delete_user(it->second->_nick); /*	Supprime l'user de la liste du channel	*/
					it->second->_channels.erase(it2);			 /*	Supprime le channel de la liste de l'user	*/
					return;
				}
			}
			return display_reply(ERR_USERNOTINCHANNEL, _command[2].c_str(), _command[1].c_str());
		}
	}
	return display_reply(ERR_NOSUCHNICK, _command[2].c_str());
}

void command::INVITE()
{
	int i = 0;
	user *invited;
	if (_command.size() < 2)
	{
		return display_reply(ERR_NEEDMOREPARAMS, _command[0].c_str());
	}
	if (_user->_mode.find('o') == std::string::npos)
		return display_reply(ERR_CHANOPRIVSNEEDED, _command[2].c_str());

	for (std::map<int, user *>::iterator it = _user->_serv->_users.begin(); it != _user->_serv->_users.end(); it++)
	{
		if (it->second->_nick == _command[1])
		{
			i = 1;
			invited = it->second;
		}
	}
	if (i == 0)
		return display_reply(ERR_NOSUCHNICK, _command[1].c_str());
	i = 0;
	if (_user->_channels.size() == 0)
		return display_reply(ERR_NOTONCHANNEL, _command[2].c_str());
	for (std::map<std::string, channel *>::iterator it = _user->_channels.begin(); it != _user->_channels.end(); it++)
	{
		if (it->second->_name == _command[2])
			i = 1;
	}
	if (i == 0)
		return display_reply(ERR_NOTONCHANNEL, _command[2].c_str());
	if (_user->_channels[_command[2]]->check_user(_command[1]))
		return display_reply(ERR_USERONCHANNEL, _command[1].c_str(), _command[2].c_str());

	std::string str = ":" + _user->_nick + "!" + _user->_serv->_name + "@localhost" + " INVITE " + _command[1] + " " + _command[2] + "\r\n";
	std::cout << str << std::endl;
	send(invited->_fd, str.c_str(), str.size(), 0);
	invited->_invited = _command[2];
	// send(_user->_fd, str.c_str(), str.size(), 0);
	return display_reply(RPL_INVITING, _command[1].c_str(), _command[2].c_str());
}

void command::PRIVMSG()
{
	if (_command.size() < 2)
		return display_reply(ERR_NORECIPIENT, _command[0].c_str());
	if (_command.size() < 3)
		return display_reply(ERR_NOTEXTTOSEND);
	for (std::map<int, user *>::iterator it = _user->_serv->_users.begin(); it != _user->_serv->_users.end(); it++)
	{
		if (it->second->_nick == _command[1])
		{
			std::string str = ":" + _user->_nick + "!" + _user->_serv->_name + "@localhost" + " PRIVMSG " + _command[1] + " ";
			unsigned long i = 2;
			while (i < _command.size())
			{
				str += _command[i++];
				if (i < _command.size())
					str += " ";
			}
			str += "\r\n";
			std::cout << "sending to " << it->second->_nick << " " << str << std::endl;
			send(it->second->_fd, str.c_str(), str.size(), 0);
			return;
		}
	}
	for (std::map<std::string, channel *>::iterator it = _user->_serv->_channels.begin(); it != _user->_serv->_channels.end(); it++)
	{
		if (it->first == _command[1])
		{
			std::string reply = ":" + _user->_nick + " PRIVMSG " + _command[1] + " ";
			unsigned long i = 2;
			while (i < _command.size())
			{
				reply += _command[i++];
				reply += " ";
			}
			reply += "\r\n";
			it->second->broadcast(reply, _user->_nick);
			return;
		}
	}
	return display_reply(ERR_NOSUCHNICK, _command[1].c_str());
}

void command::PING()
{
	if (_command.size() < 2)
		return display_reply(ERR_NOORIGIN);
	std::string reply = "PONG : " + _user->_serv->_name + "\r\n";
	std::cout << reply << std::endl;
	send(_user->_fd, reply.c_str(), reply.size(), 0);
}

void command::PONG()
{

	if (_command.size() < 2)
		return display_reply(ERR_NOORIGIN);
}

void command::QUIT()
{
	if (_command.size() < 2)
		return display_reply(ERR_NOORIGIN);
	std::string reply = ":" + _user->_nick + " QUIT ";
	unsigned long i = 1;
	while (i < _command.size())
	{
		reply += _command[i++];
		reply += " ";
	}
	reply += "\r\n";
	std::cout << reply << std::endl;
	send(_user->_fd, reply.c_str(), reply.size(), 0);
	_user->_status = "Disconnected";
	close(_user->_fd);
	// _user->_serv->_users.erase(_user->_fd);
}

std::ostream &operator<<(std::ostream &o, command &rhs)
{
	for (std::vector<std::string>::iterator it = rhs._command.begin(); it != rhs._command.end(); it++)
	{
		o << *it;
		if (it != rhs._command.end() - 1)
			std::cout << " ";
	}
	return (o);
}
