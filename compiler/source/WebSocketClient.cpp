import std;

#include <io/WebSocketClient.h>

#include <winerror.h>

#pragma comment(lib, "winhttp.lib")

// Haze WebSocketClient.cpp
// (c) Connor J. Link. All Rights Reserved.

#pragma message("TODO: fix this clas to work and test with browser JS!")

namespace hz
{
	bool WebSocketClient::connect(const std::wstring& url, const std::wstring& path)
	{
		static constexpr auto HOST_SIZE = 256;
		wchar_t host_name[HOST_SIZE];

		URL_COMPONENTS components = { 0 };
		components.dwStructSize = sizeof(components);
		components.lpszHostName = host_name;
		components.dwHostNameLength = HOST_SIZE;
		components.dwSchemeLength = static_cast<DWORD>(url.length());
		components.dwUrlPathLength = static_cast<DWORD>(path.length());

		WinHttpCrackUrl(url.c_str(), 0, 0, &components);

		session = WinHttpOpen(L"WinHttp WebSocket Client/1.0",
			WINHTTP_ACCESS_TYPE_NO_PROXY, nullptr, nullptr, 0);

		if (!session)
		{
			emit_error("Failed to open WinHttp session.");
			return false;
		}

		connection = WinHttpConnect(session, components.lpszHostName, components.nPort, 0);
		if (!connection)
		{
			emit_error("Failed to connect.");
			return false;
		}

		request = WinHttpOpenRequest(connection, L"GET", path.c_str(),
			nullptr, nullptr,
			nullptr,
			(components.nScheme == INTERNET_SCHEME_HTTPS) ? WINHTTP_FLAG_SECURE : 0);
		if (!request)
		{
			emit_error("Failed to open request.");
			return false;
		}

		auto result = WinHttpSetOption(request, WINHTTP_OPTION_UPGRADE_TO_WEB_SOCKET, nullptr, 0);
		if (!result)
		{
			emit_error("Failed to set WebSocket upgrade option.");
			return false;
		}

		if (!WinHttpSendRequest(request, nullptr, 0, nullptr, 0, 0, 0) ||
			!WinHttpReceiveResponse(request, nullptr))
		{
			emit_error("Failed to send or receive WebSocket handshake.");
			return false;
		}

		websocket = WinHttpWebSocketCompleteUpgrade(request, 0);
		request = nullptr; // WinHttpWebSocketCompleteUpgrade takes ownership; null here to prevent duplicate deletion
		if (!websocket) 
		{
			emit_error("Failed to complete WebSocket upgrade.");
			return false;
		}

		if (on_open)
		{
			on_open();
		}

		running = true;
		receiver = std::thread([this]
		{
			receive_loop(); 
		});

		return true;
	}

	bool WebSocketClient::send(const std::string& message)
	{
		if (websocket == nullptr)
		{
			return false;
		}

		auto res = WinHttpWebSocketSend(websocket,
			WINHTTP_WEB_SOCKET_UTF8_MESSAGE_BUFFER_TYPE,
			(PVOID)message.c_str(), (DWORD)message.length());

		return res == NO_ERROR;
	}

	void WebSocketClient::close()
	{
		if (!running)
		{
			return;
		}

		running = false;

		if (websocket != nullptr)
		{
			WinHttpWebSocketClose(websocket, WINHTTP_WEB_SOCKET_SUCCESS_CLOSE_STATUS, nullptr, 0);
			WinHttpWebSocketShutdown(websocket, WINHTTP_WEB_SOCKET_SUCCESS_CLOSE_STATUS, nullptr, 0);
		}

		if (receiver.joinable())
		{
			receiver.join();
		}

		if (websocket) WinHttpCloseHandle(websocket);
		if (request) WinHttpCloseHandle(request);
		if (connection) WinHttpCloseHandle(connection);
		if (session) WinHttpCloseHandle(session);

		websocket = nullptr;
		request = nullptr;
		connection = nullptr;
		session = nullptr;

		if (on_close)
		{
			on_close();
		}
	}

	void WebSocketClient::receive_loop()
	{
		const DWORD buffer_size = 4096;
		BYTE buffer[buffer_size] = { 0 };

		while (running)
		{
			DWORD bytes_read = 0;
			WINHTTP_WEB_SOCKET_BUFFER_TYPE buffer_type{};

			// NOTE: this function blocks until a message is received or the connection is closed
			// For this reason, this client MUST sit in another thread
			auto res = WinHttpWebSocketReceive(websocket, buffer, buffer_size, &bytes_read, &buffer_type);
			if (res != NO_ERROR)
			{
				emit_error("WebSocket receive error.");
				break;
			}

			if (buffer_type == WINHTTP_WEB_SOCKET_CLOSE_BUFFER_TYPE)
			{
				break;
			}

			if (buffer_type == WINHTTP_WEB_SOCKET_UTF8_MESSAGE_BUFFER_TYPE ||
				buffer_type == WINHTTP_WEB_SOCKET_UTF8_FRAGMENT_BUFFER_TYPE)
			{
				// this is safe because string will make a copy and not take ownership
				std::string message(reinterpret_cast<char*>(buffer), bytes_read);
				if (on_message)
				{
					on_message(message);
				}
			}
		}

		running = false;
		if (on_close)
		{
			on_close();
		}
	}

	void WebSocketClient::emit_error(const std::string& msg)
	{
		if (on_error)
		{
			on_error(msg);
		}
	}
}
