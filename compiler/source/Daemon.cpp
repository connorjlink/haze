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
		std::print("New client connected: {}", client);
	}

	void Daemon::OnMessage(SOCKET client, const std::string& message)
	{
		std::print("Received message from client {}: {}", client, message);
	}

	void Daemon::OnClose(SOCKET client)
	{
		std::print("Client disconnected: {}", client);
	}

	void Daemon::OnError(const std::string& error)
	{
		std::print("Error: {}", error);
	}
}
