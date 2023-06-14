#include "../includes/channel.hpp"

channel::channel() : _limit(-1)
{}

channel::channel(std::string name, user *user, serveur *serv): _name(name), _serv(serv){
    add_user(user);
	_limit = -1;
}

channel::~channel(){
}

void channel::add_user(user *user){
    _users.push_back(user);
    _users.back()->_mode = 2;
	user->_channels.insert(std::pair<std::string, channel *>(_name, this));
    std::string str = user->_nick;
    for (std::list <std::string>::iterator it = _history.begin(); it != _history.end(); it++)
        send(user->_fd, it->c_str(), it->size(), 0);
    str += "\033[0;33m has joined the channel: \033[0m";
    str += _name;
    str += "\n";
    broadcast(str);
    if (_history.size() > 20)
        _history.pop_front();
    _history.push_back(str);
}

void channel::delete_user(user *name)
{
    for (std::vector<user *>::iterator it = _users.begin(); it != _users.end(); it++)
    {
        if ((*it)->_nick == name->_nick)
        {
            std::vector<user *>::iterator at = it;
            at++;
            _users.erase(it, at);
            return ;
        }
    }
}

void channel::broadcast(std::string response){
    for (std::vector <user *>::iterator it = _users.begin(); it != _users.end(); it++)
        send((*it)->_fd, response.c_str(), response.size(), 0);
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
	if (str[0] == "\n")
		return ;
    std::string name;
    name = user->_nick;
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
    broadcast(name);
}

void channel::addMode(char c, std::string param)
{
	if (c == 'l')
		setLimit(atoi(param.c_str()));
	if (c == 'k')
		setKey(param);
	if (c == 'o')
	{
		for (std::vector<user *>::iterator it = _operators.begin(); it != _operators.end(); it++)
			if ((*it)->_nick == param)
				add_operator(*it);
	}
	else
	{
		_mode.push_back(c);
	}
}

void channel::deleteMode(char mode, std::string param)
{
	if (mode == 'l')
		removeLimit();
	if (mode == 'k')
		deleteKey();
	if (mode == 'o')
	{
		for (std::vector<user *>::iterator it = _operators.begin(); it != _operators.end(); it++)
			if ((*it)->_nick == param)
				delete_operator(*it);
	}
	else
	{
		for (std::vector <char>::iterator it = _mode.begin(); it != _mode.end(); it++)
			if (*it == mode)
				_mode.erase(it);
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
	for(std::vector<char>::iterator it = _mode.begin(); it != _mode.end(); it++)
		if (*it == m)
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

/////////// delete le channel si vide
