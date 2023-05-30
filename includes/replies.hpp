#ifndef REPLIES_HPP
# define REPLIES_HPP

# define RPL_WELCOME "001 Welcome to the Internet Relay Network <nick>!<user>@<host>"
# define RPL_YOURHOST "002 Your host is <servername>, running version <ver>"
# define RPL_CREATED "003 This server was created <date>"
# define RPL_MYINFO "004 <servername> <version> <available user modes> <available channel modes>"

// USER
# define ERR_NEEDMOREPARAMS "461 <command> :Not enough parameters"
# define ERR_ALREADYREGISTRED "462 :Unauthorized command (already registered)"

// NICK
# define ERR_NONICKNAMEGIVEN "431 :No nickname given"
# define ERR_NICKNAMEINUSE "433 <nick> :Nickname is already in use"
# define ERR_ERRONEUSNICKNAME "432 <nick> :Erroneous nickname"
# define ERR_RESTRICTED "484 :Your connection is restricted!"

// OPER
# define RPL_YOUREOPER "381 :You are now an IRC operator"
# define ERR_PASSWDMISMATCH "464 :Password incorrect"
# define ERR_NOOPERHOST "491 :No O-lines for your host"

// MODE
# define ERR_USERSDONTMATCH "502 :Cannot change mode for other users"
# define RPL_UMODEIS "221 <user mode string>"
# define ERR_UMODEUNKNOWNFLAG "501 :Unknown MODE flag"

# define ERR_KEYSET "467 <channel> :Channel key already set"
# define ERR_NOCHANMODES "477 <channel> :Channel doesn't support modes"
# define ERR_USERNOTINCHANNEL "441 <nick> <channel> :They aren't on that channel"
# define ERR_UNKNOWNMODE "472 <char> :is unknown mode char to me for <channel>"
# define RPL_CHANNELMODEIS "324 <channel> <mode> <mode params>"

# define RPL_INVITELIST "346 <channel> <invite mask>"
# define RPL_ENDOFINVITELIST "347 <channel> :End of channel invite list"
// # define RPL_EXCEPTLIST "348 <channel> <exception mask>"
// # define RPL_ENDOFEXCEPTLIST "349 <channel> :End of channel exception list"
# define RPL_BANLIST "367 <channel> <ban mask>"
# define RPL_ENDOFBANLIST "368 <channel> :End of channel ban list"

// SQUIT
# define ERR_NOPRIVILEGES "481 :Permission Denied- You're not an IRC operator"
# define ERR_NOSUCHSERVER "402 <server name> :No such server"

// JOIN
# define ERR_BANNEDFROMCHAN "474 <channel> :Cannot join channel (+b)"
# define ERR_INVITEONLYCHAN "473 <channel> :Cannot join channel (+i)"
# define ERR_BADCHANNELKEY  "475 <channel> :Cannot join channel (+k)"
# define ERR_CHANNELISFULL "471 <channel> :Cannot join channel (+l)"
# define ERR_BADCHANMASK "476 <channel> :Bad Channel Mask"
# define ERR_NOSUCHCHANNEL "403 <channel name> :No such channel"
# define ERR_TOOMANYCHANNELS "405 <channel name> :You have joined too many channels"
# define ERR_TOOMANYTARGETS "407 <target> :<error code> recipients. <abort message>"
# define ERR_UNAVAILRESOURCE "437 <nick/channel> :Nick/channel is temporarily unavailable"

// LIST
#define RPL_LIST "322 <channel> <# visible> :<topic>"
#define RPL_LISTEND "323 :End of LIST"

// NAMES
# define RPL_NAMREPLY "353 <( '=' / '*' )> <channel> :<[ '@' / '+' ] nick *( ' ' [ '@' / '+' ] nick )>"
# define RPL_ENDOFNAMES "366 <channel> :End of NAMES list"

// INVITE
# define ERR_NOSUCHNICK "401 <nickname> :No such nick/channel"
# define ERR_NOTONCHANNEL "442 <channel> :You're not on that channel"
# define ERR_USERONCHANNEL "443 <user> <channel> :is already on channel"
# define ERR_CHANOPRIVSNEEDED "482 <channel> :You're not channel operator"
# define RPL_INVITING "341 <channel> <nick>"
# define RPL_AWAY "301 <nick> :<away message>"

// PING PONG
# define ERR_NOORIGIN "409 :No origin specified"

// PRIVMSG
# define ERR_NORECIPIENT "411 :No recipient given (<command>)"
# define ERR_NOTEXTTOSEND "412 :No text to send"
# define ERR_CANNOTSENDTOCHAN "404 <channel> :Cannot send to channel"
# define ERR_NOTOPLEVEL "413 :No toplevel domain specified"
# define ERR_WILDTOPLEVEL "414 :Wildcard in toplevel domain"
# define ERR_TOOMANYTARGETS "407 <target> :<error code> recipients. <abort message>"
# define ERR_NOSUCHNICK "401 <nickname> :No such nick/channel"

# define RPL_NOTOPIC "331 <channel> :No topic is set"
# define RPL_TOPIC "332 <channel> :<topic>"

# define ERR_NOTREGISTERED "451 :You have not registered" // todo dans exec_command
			             	
# define ERR_CANTKILLSERVER "483 :You can't kill a server!"         	                 

//AWAY
# define RPL_UNAWAY "305 :You are no longer marked as being away"
# define RPL_NOWAWAY "306 :You have been marked as being away"

//WHO
# define RPL_WHOREPLY "352 <channel> <user> <host> <server> <nick> <state> :0 <real name>"  	           	
# define RPL_ENDOFWHO "315 <name> :End of WHO list"

// WHOIS
# define RPL_WHOISUSER "331 <nick> <user> <host> * :<real name>"
# define RPL_WHOISCHANNELS "319 <nick> :<INFO>"
# define RPL_WHOISSERVER "312 <nick> <server> :<server info>"
# define RPL_WHOISOPERATOR "313 <nick> :is an IRC operator"
# define RPL_WHOISIDLE "317 <nick> <integer> :seconds idle"

// CMD CLEAR TERMINAL
# define CLEAR_TERM "\x1B[2J\x1B[H"

// COLORS

# define RED "\033[0;31m";
# define GREEN "\033[0;32m";
# define YELLOW "\033[0;33m";
# define BLUE "\033[0;34m";
# define PURPLE "\033[0;35m";
# define AZUR "\033[0;36m";
# define GREY "\033[0;37m";

# define CLOSE "\033[0m";

#endif
