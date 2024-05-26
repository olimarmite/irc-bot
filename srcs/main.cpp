#include "irc_bot.hpp"
#include <exception>
#include <iostream>

int main(int argc, char **argv)
{

	if (argc < 3)
	{
		std::cerr << "Usage: <host> <port> [password]" << std::endl;
		return (1);
	}

	std::string host = argv[1];
	std::string port = argv[2];
	std::string password = argc == 4 ? argv[3] : "";

	IrcBot bot(host, port, password);
	try {
		bot.bot_connect();

		bool running = true;
		while (running)
		{
			running = bot.tick();
		}
	} catch (std::exception &e) {
		std::cerr << "Error: " << e.what() << std::endl;

	}

}
