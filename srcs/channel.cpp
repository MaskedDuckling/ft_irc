#include "../includes/channel.hpp"

channel::channel() : _limit(-1)
{}

channel::channel(std::string name, user *user, serveur *serv): _name(name), _serv(serv), _mode(""), _key(""){
    add_user(user, 2);
	_limit = -1;
}

channel::~channel(){
}

int channel::check_user(std::string nick)
{
	for (std::vector<user *>::iterator it = _users.begin(); it != _users.end(); it++)
		if ((*it)->_nick == nick)
			return (1);
	return (0);
}

void channel::add_user(user *user, int mode)
{
    _users.push_back(user);
    _users.back()->_mode = 2;
	user->_channels.insert(std::pair<std::string, channel *>(_name, this));
	user->_chan_name.push_back(_name);
	if (mode == 1)
    {
		std::string str = ":" + user->_nick + "!" + _serv->_name + "@localhost" + " JOIN " + _name + "\r\n";
		// if (_users.size() == 1)
			broadcast(str, "");
		// else
			// broadcast(str, user->_nick);
		list_users();
	}
	if (mode == 2)
    {
		_operators.push_back(user);
		std::string str = ":" + user->_nick + "!" + _serv->_name + "@localhost" + " JOIN " + _name + "\r\n";
		// if (_users.size() == 1)
			broadcast(str, "");
		// else
			// broadcast(str, user->_nick);
		list_users();
	}
}

void channel::delete_user(std::string nick)
{
	for (std::vector<user *>::iterator it = _users.begin(); it != _users.end(); it++)
	{
		if ((*it)->_nick == nick)
		{
			std::vector<user *>::iterator at = it;
			at++;
			_users.erase(it, at);
			list_users();
			return ;
		}
	}
	if (_users.size() == 0)
		_serv->_channels.erase(_name);
}

void channel::broadcast(std::string response, std::string usr){
    for (std::vector <user *>::iterator it = _users.begin(); it != _users.end(); it++)
	{
		if (usr != (*it)->_nick){
			std::cout << "broadcasting to " << (*it)->_nick << " " << response << std::endl;
        	send((*it)->_fd, response.c_str(), response.size(), 0);
		}
	}
}

void channel::list_users(){
    int users = 0;
    for (std::vector<user *>::iterator it = _users.begin(); it != _users.end(); it++){
        std::cout << (*it)->_nick << std::endl;
        users++;
    }
    std::cout << users << " users in channel " << std::endl;
}

void channel::print_msg(std::vector<std::string> str, user *user)
{
    std::string name = "\033[0;33m#" + _name + " \033[0m" + user->_nick;
    name += " : ";
    for (std::vector <std::string>::iterator it = str.begin(); it != str.end(); it++)
    {
        name += *it;
        name += " ";
    }
    name += "\n";
    if (_history.size() > 20)
        _history.pop_front();
    _history.push_back(name);
    broadcast(name, user->_nick);
}

void channel::print_history(user *user)
{
	for (std::list<std::string>::iterator it = _history.begin(); it != _history.end(); it++)
	{
		send(user->_fd, it->c_str(), it->size(), 0);
	}
}

int channel::getMode(char c)
{
	if (_mode.find(c) != std::string::npos)
		return (1);
	return (0);
}

void channel::addMode(char c, std::string param)
{
	if (c == 'l')
		setLimit(atoi(param.c_str()));
	else if (c == 'k')
		setKey(param);
	else if (c == 'o')
	{
		for (std::vector<user *>::iterator it = _users.begin(); it != _users.end(); it++)
 		{
			if ((*it)->_nick == param)
			{
 				add_operator(*it);
 				return ;
 			}
 		}
 	}
 	else
	{
 		if (c == 'l')
		{
 			deleteMode('l', param);
 			setLimit(atoi(param.c_str()));
 		}
 		_mode.push_back(c);
	}
}

void channel::deleteMode(char mode, std::string param)
{
	std::cout << "delete mode " << mode << std::endl;
	if (mode == 'l')
	{
 		removeLimit();
		if (_mode.find(mode) != std::string::npos)
			_mode.erase(_mode.find(mode), 1);
	}
	else if (mode == 'k')
	{
 		deleteKey();
		if (_mode.find(mode) != std::string::npos)
			_mode.erase(_mode.find(mode), 1);
	}
	else if (mode == 'o')
	{
		for (std::vector<user *>::iterator it = _operators.begin(); it != _operators.end(); it++)
 			if ((*it)->_nick == param)
 			{
 				delete_operator(*it);
 				return ;
			}
	}
	else
	{
 		_mode.erase(_mode.find(mode,1));
	}
}

void channel::setLimit(int limit)
{
	_limit = limit;
}

void channel::removeLimit()
{
	_limit = -1;
}

unsigned int channel::getNbUsers()
{
	return (_users.size());
}

void channel::setKey(std::string key)
{
	_key = key;
}

void channel::deleteKey()
{
	_key = "";
}

void channel::add_operator(user *user)
{
	_operators.push_back(user);
}

void channel::delete_operator(user *name)
{
	for (std::vector<user *>::iterator it = _operators.begin(); it != _operators.end(); it++)
	{
		if ((*it)->_nick == name->_nick)
		{
			std::vector<user *>::iterator at = it;
			at++;
			_operators.erase(it, at);
			return ;
		}
	}
}

int channel::isKeySet()
{
	if (_key == "")
		return 1;
	else
		return (0);
}

int channel::isModeSet(char m)
{
	if (_mode.find(m) != std::string::npos)
		return (1);
	return (0);
}

int channel::checkOper(std::string name)
{
	for (std::vector<user *>::iterator it = _operators.begin(); it != _operators.end(); it++)
		if ((*it)->_nick == name)
			return (1);
	return (0);
}

/*int channel::change_channel_mode(char sign, char mode, std::vector<std::string> command){

	std::string modes = "itkol";
	std::cout << "mode : " << mode << std::endl;
	if (modes.find(mode) == std::string::npos)
		return (-1);
	std::cout << "mode : " << mode << std::endl;
	if (sign == '+'){
		if (_mode.find(mode) != std::string::npos)
			return (0);
		_mode += mode;
		if (mode == 'l' && command.size() > 3){
			std::stringstream sstream(command[3]);
			int i;
			sstream >> i;
			if (sstream.fail() || !sstream.eof() || i < 0){
				std::cout << "Error invalid port" << std::endl;
				return 0;
			}
			_limit = i;
		}
		if (mode == 'k')
			_key = command[3];
		return (1);
	}
	else if (sign == '-' && _mode.find(mode) != std::string::npos)
	{
		_mode.erase(_mode.find(mode), 1);
		return (1);
	}
	return (0);
}*/