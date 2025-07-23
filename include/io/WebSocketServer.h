#ifndef HAZE_WEBSOCKETSERVER_H
#define HAZE_WEBSOCKETSERVER_H

#include <WinSock2.h>

// Haze WebSocketServer.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	class WebSocketServer
	{
	private:
		std::function<void(SOCKET)> on_open;
		std::function<void(SOCKET, const std::string&)> on_message;
		std::function<void(SOCKET)> on_close;
		std::function<void(const std::string&)> on_error;

	private:
		std::atomic<bool> running{ false };
		std::thread accept_thread;
		SOCKET listen_socket = INVALID_SOCKET;
		std::vector<std::thread> client_threads;

	public:
		bool start(std::uint16_t);
		void stop();
		void accept_loop();
		void handle_client(SOCKET);
		void emit_error(const std::string&) const;

	public:
		template<typename OnOpen, typename OnMessage, typename OnClose, typename OnError>
		WebSocketServer(OnOpen onopen, OnMessage onmessage, OnClose onclose, OnError onerror)
			: on_open(std::move(onopen)), on_message(std::move(onmessage)), on_close(std::move(onclose)), on_error(std::move(onerror))
		{
		}
	};
}

#endif
