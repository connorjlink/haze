#ifndef HAZE_WEBSOCKCLIENT_H
#define HAZE_WEBSOCKCLIENT_H

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <winhttp.h>
#undef ERROR
#undef VOID
#undef NULL

// Haze WebSocketClient.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	class WebSocketClient
	{
	public:
		std::function<void()> on_open;
		std::function<void(const std::string&)> on_message;
		std::function<void()> on_close;
		std::function<void(const std::string&)> on_error;

	private:
		HINTERNET session;
		HINTERNET connection;
		HINTERNET request;
		HINTERNET websocket;
		std::atomic<bool> running;
		std::thread receiver;

	public:
		bool connect(const std::wstring&, const std::wstring& = L"/");
		bool send(const std::string&);
		void close();

	public:
		void receive_loop();
		void emit_error(const std::string&);

	public:
		inline bool is_running(void) const
		{
			return running;
		}

	public:
		WebSocketClient()
			: session(nullptr), connection(nullptr), request(nullptr), websocket(nullptr), running(false)
		{
		}

		~WebSocketClient()
		{
			close();
		}
	};
}

#endif
