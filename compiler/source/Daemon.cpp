import std;

#include <daemon/Daemon.h>

// Haze Daemon.cpp
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	Daemon::Daemon(std::uint16_t port)
		: server{ &OnOpen, &OnMessage, &OnClose, &OnError }
	{
		server.start(port);
	}
#pragma message("TODO: implement Daemon class to handle WebSocket connections and messages")
	void Daemon::OnOpen(SOCKET client)
	{
		std::cout << "New client connected: " << client << std::endl;
	}

	void Daemon::OnMessage(SOCKET client, const std::string& message)
	{
		std::cout << "Received message from client " << client << ": " << message << std::endl;
	}

	void Daemon::OnClose(SOCKET client)
	{
		std::cout << "Client disconnected: " << client << std::endl;
	}

	void Daemon::OnError(const std::string& error)
	{
		std::cerr << "Error: " << error << std::endl;
	}
}
