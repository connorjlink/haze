import std;

#include <io/WebSocketServer.h>

#include <websocket.h>
#include <ws2tcpip.h>

// Haze WebSocketServer.h
// (c) Connor J. Link. All Rights Reserved.

#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "websocket.lib")

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
		std::string req;
		char buffer[4096];
		while (true)
		{
			int received = recv(client, buffer, sizeof(buffer), 0);
			if (received <= 0)
			{
				closesocket(client);
				return;
			}
			req.append(buffer, received);

			// HTTP request ends with two breaks
			if (req.find("\r\n\r\n") != std::string::npos)
				break;
		}

		std::vector<std::string> header_names;
		std::vector<std::string> header_values;
		std::istringstream iss(req);
		std::string line;

		// Saltar la línea de petición (GET / HTTP/1.1)
		if (!std::getline(iss, line))
		{
			closesocket(client);
			return;
		}

		while (std::getline(iss, line))
		{
			// El final de las cabeceras HTTP es una línea vacía
			if (line == "\r" || line.empty())
				break;

			const auto colon = line.find(':');
			if (colon == std::string::npos)
				continue; // Saltar líneas mal formateadas

			std::string name = line.substr(0, colon);
			std::string value = line.substr(colon + 1);

			// trim leading whitespace
			value.erase(0, value.find_first_not_of(" \t\r\n"));
			// trim trailing whitespace
			value.erase(value.find_last_not_of(" \t\r\n") + 1);

			// trim trailing whitespace in name también
			name.erase(name.find_last_not_of(" \t\r\n") + 1);

			if (name.empty() || value.empty())
				continue; // Saltar cabeceras vacías

			header_names.push_back(name);
			header_values.push_back(value);
		}

		std::vector<WEB_SOCKET_HTTP_HEADER> request_headers;
		for (std::size_t i = 0; i < header_names.size(); ++i)
		{
			request_headers.push_back(
				{
				const_cast<PCHAR>(header_names[i].c_str()), static_cast<ULONG>(header_names[i].size()),
				const_cast<PCHAR>(header_values[i].c_str()), static_cast<ULONG>(header_values[i].size())
			});
		}

		WEB_SOCKET_HANDLE websocket_handle = nullptr;
		if (WebSocketCreateServerHandle(nullptr, 0, &websocket_handle) != S_OK)
		{
			closesocket(client);
			return;
		}

		PWEB_SOCKET_HTTP_HEADER response_headers = nullptr;
		ULONG response_header_count = 0;
		if (WebSocketBeginServerHandshake(
			websocket_handle,
			nullptr, 
			nullptr, 0,
			request_headers.data(), static_cast<ULONG>(request_headers.size()),
			&response_headers, &response_header_count) != S_OK)
		{
			WebSocketDeleteHandle(websocket_handle);
			closesocket(client);
			return;
		}

		std::ostringstream oss;
		oss << "HTTP/1.1 101 Switching Protocols\r\n";
		for (ULONG i = 0; i < response_header_count; ++i)
		{
			oss.write(response_headers[i].pcName, response_headers[i].ulNameLength);
			oss << ": ";
			oss.write(response_headers[i].pcValue, response_headers[i].ulValueLength);
			oss << "\r\n";
		}
		oss << "\r\n";
		auto response = oss.str();
		send(client, response.c_str(), static_cast<int>(response.size()), 0);

		if (WebSocketEndServerHandshake(websocket_handle) != S_OK)
		{
			WebSocketDeleteHandle(websocket_handle);
			closesocket(client);
			return;
		}

		if (WebSocketReceive(websocket_handle, nullptr, nullptr) != S_OK)
		{
			WebSocketDeleteHandle(websocket_handle);
			closesocket(client);
			return;
		}

		if (on_open)
		{
			on_open(client);
		}

		bool local_running = true;
		while (local_running && running)
		{
			fd_set readfds{};
			FD_ZERO(&readfds);
			FD_SET(client, &readfds);

			timeval timeout{};
			// maximum 1 second interval
			timeout.tv_sec = 1;
			timeout.tv_usec = 0;

			int ready = select(0, &readfds, nullptr, nullptr, &timeout);
			if (ready < 0)
			{
				break;
			}
			if (ready == 0)
			{
				continue;
			}

			WEB_SOCKET_ACTION action;
			WEB_SOCKET_BUFFER_TYPE buffer_type;
			WEB_SOCKET_BUFFER buffer;
			ULONG bufferCount = 1;
			PVOID appContext = nullptr, action_context = nullptr;

			if (WebSocketGetAction(
				websocket_handle,
				WEB_SOCKET_RECEIVE_ACTION_QUEUE,
				&buffer, &bufferCount,
				&action, &buffer_type,
				&appContext, &action_context) != S_OK)
			{
				break;
			}

			switch (action)
			{
				case WEB_SOCKET_RECEIVE_FROM_NETWORK_ACTION:
				{
					int bytes_received = recv(client, (char*)buffer.Data.pbBuffer, buffer.Data.ulBufferLength, 0);
					if (bytes_received <= 0)
					{
						local_running = false;
						break;
					}
					WebSocketCompleteAction(websocket_handle, action_context, bytes_received);
					break;
				}
				case WEB_SOCKET_INDICATE_RECEIVE_COMPLETE_ACTION:
				{
					if (buffer_type == WEB_SOCKET_UTF8_MESSAGE_BUFFER_TYPE && on_message)
					{
						std::string message(reinterpret_cast<char*>(buffer.Data.pbBuffer), buffer.Data.ulBufferLength);
						on_message(client, message);
					}
					else if (buffer_type == WEB_SOCKET_CLOSE_BUFFER_TYPE)
					{
						local_running = false;
					}
					WebSocketCompleteAction(websocket_handle, action_context, 0);

					if (local_running && running)
					{
						if (WebSocketReceive(websocket_handle, nullptr, nullptr) != S_OK)
						{
							local_running = false;
						}
					}
					break;
				}
				case WEB_SOCKET_NO_ACTION:
				{
					std::this_thread::sleep_for(std::chrono::milliseconds(10));
					break;
				}
				default:
					WebSocketCompleteAction(websocket_handle, action_context, 0);
					break;
			}
		}

		if (on_close)
		{
			on_close(client);
		}

		WebSocketDeleteHandle(websocket_handle);
		closesocket(client);
	}

	void WebSocketServer::emit_error(const std::string& message) const
	{
		if (on_error)
		{
			on_error(message);
		}
		std::cerr << message << std::endl;
	}
}
