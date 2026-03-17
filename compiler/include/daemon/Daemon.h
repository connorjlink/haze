#ifndef HAZE_DAEMON_H
#define HAZE_DAEMON_H

#include <data/DependencyInjector.h>
#include <io/WebSocketServer.h>

// Haze Daemon.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	class Daemon
		: public SingletonTag<Daemon>
	{
	private:
		WebSocketServer server;

	private:
		static void OnOpen(SOCKET);
		static void OnMessage(SOCKET, const std::string&);
		static void OnClose(SOCKET);
		static void OnError(const std::string&);

	public:
		Daemon(std::uint16_t);
	};
}

#endif
