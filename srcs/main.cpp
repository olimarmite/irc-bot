#include <cerrno>
#include <cstring>
#include <iostream>
#include <ostream>
#include <sstream>
#include <stdexcept>
#include <streambuf>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <unistd.h>
#include <cstring>


#define CMD_PASS(password) std::string("PASS ") + password + "\r\n"
#define CMD_NICK(nickname) std::string("NICK ") + nickname + "\r\n"
#define CMD_USER(username, realname) std::string("USER ") + username + " 0 * " + realname + "\r\n"


#define BOT_NAME "Tierry"

void send_msg(int fd, std::string msg)
{
	send(fd, msg.c_str(), msg.length(), MSG_DONTWAIT | MSG_NOSIGNAL);
}

int main()
{
	std::string server_password = "";

	struct addrinfo hints;
	struct addrinfo *addrinfos;

	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	if (getaddrinfo("punch.wa.us.dal.net", "6667", &hints, &addrinfos) != 0)
	{
		throw std::runtime_error(gai_strerror(errno));
	}

	int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (socket_fd < 0)
	{
		throw std::runtime_error(strerror(errno));
	}

	if (connect(socket_fd, addrinfos->ai_addr, addrinfos->ai_addrlen) != 0)
	{
		throw std::runtime_error(strerror(errno));
	}

	int readed_count = 0;

	char buff[1025];

	if (server_password.empty() == false)
		send_msg(socket_fd, CMD_PASS(server_password));

	send_msg(socket_fd, CMD_USER(BOT_NAME, BOT_NAME));
	send_msg(socket_fd, CMD_NICK(BOT_NAME));



	// for (int i = 0; i < 20; i++)
	// {
	// 	send_msg(socket_fd, "PING\r\n");
	// 	// send_msg(socket_fd, "PING\r\n");
// }
	while (readed_count != -1) {
		memset(&buff, 0, 1025);
		readed_count = read(socket_fd, buff, 1024);
		std::cout<< buff << std::flush;
	}
	close(socket_fd);
	freeaddrinfo(addrinfos);
}
