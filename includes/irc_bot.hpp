#pragma once

#include <string>

class IrcBot
{

	private:
		std::string _host;
		std::string _port;
		std::string _server_password;
		int _socket_fd;
		void send_msg(std::string msg);
		void on_connected();
		void on_disconnected();
		void send_bad_apple();

	public:
		IrcBot(std::string host, std::string port, std::string server_password);
		~IrcBot();

		void bot_connect();
		bool tick();

};
