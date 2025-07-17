#ifndef HAZE_WEBSOCKETSERVER_H
#define HAZE_WEBSOCKETSERVER_H

// Haze WebSocketServer.h
// (c) Connor J. Link. All Rights Reserved.

#include <WinSock2.h>

namespace hz
{
	class WebSocketServer
	{
	public:
		std::function<void(SOCKET)> on_open;
		std::function<void(SOCKET, const std::string&)> on_message;
		std::function<void(SOCKET)> on_close;
		std::function<void(const std::string&)> on_error;

		std::atomic<bool> running{ false };
		std::thread accept_thread;
		SOCKET listen_socket = INVALID_SOCKET;
		std::vector<std::thread> client_threads;

		bool start(std::uint16_t port);
		void stop();
		void accept_loop();
		void handle_client(SOCKET client);
		void emit_error(const std::string&);
	};
}

#endif
