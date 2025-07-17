import std;

#include <io/WebSocketServer.h>

#include <websocket.h>
#include <ws2tcpip.h>

// Haze WebSocketServer.h
// (c) Connor J. Link. All Rights Reserved.

#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "websocket.lib")

#error TODO CLEAN UP THIS CLASS AND TEST WITH BROWSER CLIENT!

namespace hz
{
	bool WebSocketServer::start(std::uint16_t port)
	{
		WSADATA wsaData{};
		if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		{
			emit_error("WSAStartup failed");
			return false;
		}

		listen_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (listen_socket == INVALID_SOCKET)
		{
			emit_error("socket() failed");
			return false;
		}

		sockaddr_in service{};
		service.sin_family = AF_INET;
		service.sin_addr.s_addr = htonl(INADDR_ANY);
		service.sin_port = htons(port);

		if (bind(listen_socket, reinterpret_cast<SOCKADDR*>(&service), sizeof(service)) == SOCKET_ERROR)
		{
			emit_error("bind() failed");
			closesocket(listen_socket);
			return false;
		}

		if (listen(listen_socket, SOMAXCONN) == SOCKET_ERROR)
		{
			emit_error("listen() failed");
			closesocket(listen_socket);
			return false;
		}

		running = true;
		accept_thread = std::thread([this]
		{
			accept_loop();
		});

		return true;
	}

	void WebSocketServer::stop()
	{
		running = false;
		if (listen_socket != INVALID_SOCKET)
		{
			closesocket(listen_socket);
			listen_socket = INVALID_SOCKET;
		}

		if (accept_thread.joinable())
		{
			accept_thread.join();
		}

		for (auto& t : client_threads)
		{
			if (t.joinable())
			{
				t.join();
			}
		}

		WSACleanup();
	}

	void WebSocketServer::accept_loop()
	{
		while (running)
		{
			SOCKET client = accept(listen_socket, nullptr, nullptr);
			if (client == INVALID_SOCKET)
			{
				continue;
			}
			client_threads.emplace_back([this, client]()
			{
				handle_client(client);
			});
		}
	}

	void WebSocketServer::handle_client(SOCKET client)
	{
		char buffer[4096];
		int received = recv(client, buffer, sizeof(buffer), 0);
		if (received <= 0)
		{
			closesocket(client);
			return;
		}
		std::string req(buffer, received);

		// 2. Extraer headers HTTP y construir array de WEB_SOCKET_HTTP_HEADER
		std::vector<WEB_SOCKET_HTTP_HEADER> requestHeaders;
		std::istringstream iss(req);
		std::string line;
		while (std::getline(iss, line) && line != "\r")
		{
			auto colon = line.find(':');
			if (colon != std::string::npos)
			{
				std::string name = line.substr(0, colon);
				std::string value = line.substr(colon + 1);
				// Quitar espacios y \r
				while (!value.empty() && (value[0] == ' ' || value[0] == '\t')) value.erase(0, 1);
				if (!value.empty() && value.back() == '\r') value.pop_back();
				requestHeaders.push_back({
					(PCHAR)name.c_str(), (ULONG)name.size(),
					(PCHAR)value.c_str(), (ULONG)value.size()
					});
			}
		}

		// 3. Crear el handle WebSocket
		WEB_SOCKET_HANDLE wsHandle = nullptr;
		if (WebSocketCreateServerHandle(nullptr, 0, &wsHandle) != S_OK)
		{
			closesocket(client);
			return;
		}

		// 4. Handshake WebSocket
		PWEB_SOCKET_HTTP_HEADER responseHeaders = nullptr;
		ULONG responseHeaderCount = 0;
		if (WebSocketBeginServerHandshake(
			wsHandle,
			nullptr, // subprotocol
			nullptr, 0, // extensions
			requestHeaders.data(), (ULONG)requestHeaders.size(),
			&responseHeaders, &responseHeaderCount) != S_OK)
		{
			WebSocketDeleteHandle(wsHandle);
			closesocket(client);
			return;
		}

		// 5. Enviar respuesta HTTP con los headers generados por la API
		std::ostringstream oss;
		oss << "HTTP/1.1 101 Switching Protocols\r\n";
		for (ULONG i = 0; i < responseHeaderCount; ++i)
		{
			oss.write(responseHeaders[i].pcName, responseHeaders[i].ulNameLength);
			oss << ": ";
			oss.write(responseHeaders[i].pcValue, responseHeaders[i].ulValueLength);
			oss << "\r\n";
		}
		oss << "\r\n";
		std::string response = oss.str();
		send(client, response.c_str(), (int)response.size(), 0);

		if (WebSocketEndServerHandshake(wsHandle) != S_OK)
		{
			WebSocketDeleteHandle(wsHandle);
			closesocket(client);
			return;
		}

		if (on_open)
		{
			on_open(client);
		}

		// 6. Bucle de mensajes usando la API WebSocket
		bool local_running = true;
		while (local_running && running)
		{
			WEB_SOCKET_ACTION action;
			WEB_SOCKET_BUFFER_TYPE bufferType;
			WEB_SOCKET_BUFFER buffer;
			ULONG bufferCount = 1;
			PVOID appContext = nullptr, actionContext = nullptr;

			if (WebSocketGetAction(
				wsHandle,
				WEB_SOCKET_RECEIVE_ACTION_QUEUE,
				&buffer, &bufferCount,
				&action, &bufferType,
				&appContext, &actionContext) != S_OK)
			{
				break;
			}

			switch (action)
			{
			case WEB_SOCKET_RECEIVE_FROM_NETWORK_ACTION:
			{
				int n = recv(client, (char*)buffer.Data.pbBuffer, buffer.Data.ulBufferLength, 0);
				if (n <= 0)
				{
					local_running = false;
					break;
				}
				WebSocketCompleteAction(wsHandle, actionContext, n);
				break;
			}
			case WEB_SOCKET_INDICATE_RECEIVE_COMPLETE_ACTION:
			{
				if (bufferType == WEB_SOCKET_UTF8_MESSAGE_BUFFER_TYPE && on_message)
				{
					std::string msg((char*)buffer.Data.pbBuffer, buffer.Data.ulBufferLength);
					on_message(client, msg);
				}
				else if (bufferType == WEB_SOCKET_CLOSE_BUFFER_TYPE)
				{
					local_running = false;
				}
				WebSocketCompleteAction(wsHandle, actionContext, 0);
				break;
			}
			default:
				WebSocketCompleteAction(wsHandle, actionContext, 0);
				break;
			}
		}

		if (on_close)
		{
			on_close(client);
		}
		WebSocketDeleteHandle(wsHandle);
		closesocket(client);
	}

	void WebSocketServer::emit_error(const std::string& message)
	{
		if (on_error)
		{
			on_error(message);
		}
	}
}
