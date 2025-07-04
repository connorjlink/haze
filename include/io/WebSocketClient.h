#ifndef HAZE_WEBSOCKCLIENT_H
#define HAZE_WEBSOCKCLIENT_H

// Haze WebSocketClient.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	class WebSocketClient
	{
	public:
		std::function<void()> onOpen;
		std::function<void(const std::string& message)> onMessage;
		std::function<void()> onClose;
		std::function<void(const std::string& error)> onError;

		bool connect(const std::wstring& url, const std::wstring& path = L"/");
		bool send(const std::string& message);

		void close()
		{
			if (!running)
			{
				return;
			}

			running = false;

			if (hWebSocket)
			{
				WinHttpWebSocketClose(hWebSocket, WINHTTP_WEB_SOCKET_SUCCESS_CLOSE_STATUS, nullptr, 0);
				WinHttpWebSocketShutdown(hWebSocket, WINHTTP_WEB_SOCKET_SUCCESS_CLOSE_STATUS, nullptr, 0);
			}

			if (recvThread.joinable())
			{
				recvThread.join();
			}

			if (hWebSocket) WinHttpCloseHandle(hWebSocket);
			if (hRequest) WinHttpCloseHandle(hRequest);
			if (hConnect) WinHttpCloseHandle(hConnect);
			if (hSession) WinHttpCloseHandle(hSession);

			hWebSocket = nullptr;
			hRequest = nullptr;
			hConnect = nullptr;
			hSession = nullptr;

			if (onClose)
			{
				onClose();
			}
		}

	private:
		HINTERNET hSession;
		HINTERNET hConnect;
		HINTERNET hRequest;
		HINTERNET hWebSocket;

		std::atomic<bool> running;
		std::thread recvThread;

		void receiveLoop() {
			const DWORD bufferSize = 4096;
			BYTE buffer[bufferSize];

			while (running) {
				DWORD bytesRead = 0;
				WINHTTP_WEB_SOCKET_BUFFER_TYPE bufferType;

				DWORD res = WinHttpWebSocketReceive(hWebSocket, buffer, bufferSize, &bytesRead, &bufferType);
				if (res != NO_ERROR) {
					emitError("WebSocket receive error.");
					break;
				}

				if (bufferType == WINHTTP_WEB_SOCKET_CLOSE_BUFFER_TYPE) {
					break;
				}

				if (bufferType == WINHTTP_WEB_SOCKET_UTF8_MESSAGE_BUFFER_TYPE ||
					bufferType == WINHTTP_WEB_SOCKET_UTF8_FRAGMENT_BUFFER_TYPE) {
					std::string message(reinterpret_cast<char*>(buffer), bytesRead);
					if (onMessage) onMessage(message);
				}
			}

			running = false;
			if (onClose) onClose();
		}

		void emitError(const std::string& msg) {
			if (onError) onError(msg);
		}

	public:
		WebSocketClient()
			: hSession(NULL), hConnect(NULL), hRequest(NULL), hWebSocket(NULL), running(false)
		{
		}

		~WebSocketClient()
		{
			close();
		}
	};
}

#endif
