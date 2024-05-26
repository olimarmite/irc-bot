#include <cerrno>
#include <cstring>
#include <iostream>
#include <ostream>
#include <stdexcept>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <unistd.h>
#include <cstring>
#include <fstream>


#include "irc_bot.hpp"


#define BOT_NAME "Tierry"

#define CMD_PASS(password) std::string("PASS ") + password + "\r\n"
#define CMD_NICK(nickname) std::string("NICK ") + nickname + "\r\n"
#define CMD_USER(username, realname) std::string("USER ") + username + " 0 * " + realname + "\r\n"
#define CMD_JOIN(channel) std::string("JOIN ") + channel + "\r\n"
#define CMD_PRIVMSG(target, message) std::string("PRIVMSG ") + target + " :" + std::string(message) + "\r\n"


IrcBot::IrcBot(std::string host, std::string port, std::string server_password)
{
	this->_host = host;
	this->_port = port;
	this->_server_password = server_password;
	this->_socket_fd = -1;
}
IrcBot::~IrcBot()
{
}
void IrcBot::bot_connect()
{
	struct addrinfo hints;
	struct addrinfo *addrinfos;

	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	// if (getaddrinfo("punch.wa.us.dal.net", "6667", &hints, &addrinfos) != 0)
	if (getaddrinfo(_host.c_str(), _port.c_str(), &hints, &addrinfos) != 0)
	{
		throw std::runtime_error(gai_strerror(errno));
	}

	_socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (_socket_fd < 0)
	{
		freeaddrinfo(addrinfos);
		throw std::runtime_error(strerror(errno));
	}

	if (connect(_socket_fd, addrinfos->ai_addr, addrinfos->ai_addrlen) != 0)
	{
		freeaddrinfo(addrinfos);
		close(_socket_fd);
		throw std::runtime_error(strerror(errno));
	}
	freeaddrinfo(addrinfos);
	on_connected();
}

void IrcBot::send_msg(std::string msg)
{
	if (send(_socket_fd, msg.c_str(), msg.length(), MSG_DONTWAIT | MSG_NOSIGNAL) < 0)
	{
		throw std::runtime_error(strerror(errno));
	}

}

void IrcBot::on_connected()
{
	if (_server_password.empty() == false)
		send_msg(CMD_PASS(_server_password));
	send_msg(CMD_USER(BOT_NAME, BOT_NAME));
	send_msg(CMD_NICK(BOT_NAME));
	send_msg(CMD_JOIN("#bad_apple_bot"));
	send_msg(CMD_PRIVMSG("#bad_apple_bot", "hellow world"));
}

void IrcBot::on_disconnected()
{
	std::cout << "Disconnected" << std::endl;
}


void IrcBot::send_bad_apple()
{
	std::ifstream file("./bad_apple_conversion/result.txt");
	std::string msg;


	if (!file)
	{
		throw std::runtime_error("Failed to open file");
	}

	while (std::getline(file, msg))
	{
		send_msg(CMD_PRIVMSG("#bad_apple_bot", msg));
		if (msg.find("-") != std::string::npos)
			usleep(30000);
	}
	file.close();
}

bool IrcBot::tick()
{
	int readed_count = 0;
	char buff[1025];

	readed_count = read(_socket_fd, buff, 1024);
	if (readed_count == 0)
	{
		close(_socket_fd);
		on_disconnected();
		return false;
	}
	else if (readed_count < 0)
	{
		close(_socket_fd);
		on_disconnected();
		throw std::runtime_error(strerror(errno));
	}

	buff[readed_count] = '\0';
	std::cout << buff << std::flush;

	if (std::string(buff).find("PRIVMSG #bad_apple_bot :!apple") != std::string::npos)
	{
		try {
			send_bad_apple();
		} catch (std::exception &e) {
			close(_socket_fd);
			on_disconnected();
			std::cerr << "Error: " << e.what() << std::endl;
		}
	}
	return true;
}
