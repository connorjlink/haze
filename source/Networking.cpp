import std;

#include "WebSocketClient.h"

#include <windows.h>
#include <winhttp.h>

#pragma comment(lib, "winhttp.lib")

// Haze WebSocketClient.cpp
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	bool WebSocketClient::connect(const std::wstring& url, const std::wstring& path = L"/")
	{
		URL_COMPONENTS urlComp = { 0 };
		wchar_t hostName[256];
		urlComp.dwStructSize = sizeof(urlComp);
		urlComp.lpszHostName = hostName;
		urlComp.dwHostNameLength = _countof(hostName);
		urlComp.dwSchemeLength = url.length();
		urlComp.dwUrlPathLength = path.length();

		WinHttpCrackUrl(url.c_str(), 0, 0, &urlComp);

		hSession = WinHttpOpen(L"WinHttp WebSocket Client/1.0",
			WINHTTP_ACCESS_TYPE_NO_PROXY, WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, 0);

		if (!hSession) {
			emitError("Failed to open WinHttp session.");
			return false;
		}

		hConnect = WinHttpConnect(hSession, urlComp.lpszHostName,
			urlComp.nPort, 0);
		if (!hConnect) {
			emitError("Failed to connect.");
			return false;
		}

		hRequest = WinHttpOpenRequest(hConnect, L"GET", path.c_str(),
			NULL, WINHTTP_NO_REFERER,
			WINHTTP_DEFAULT_ACCEPT_TYPES,
			(urlComp.nScheme == INTERNET_SCHEME_HTTPS) ? WINHTTP_FLAG_SECURE : 0);
		if (!hRequest) {
			emitError("Failed to open request.");
			return false;
		}

		BOOL bResult = WinHttpSetOption(hRequest,
			WINHTTP_OPTION_UPGRADE_TO_WEB_SOCKET, NULL, 0);
		if (!bResult) {
			emitError("Failed to set WebSocket upgrade option.");
			return false;
		}

		if (!WinHttpSendRequest(hRequest, WINHTTP_NO_ADDITIONAL_HEADERS, 0,
			WINHTTP_NO_REQUEST_DATA, 0, 0, 0) ||
			!WinHttpReceiveResponse(hRequest, NULL)) {
			emitError("Failed to send or receive WebSocket handshake.");
			return false;
		}

		hWebSocket = WinHttpWebSocketCompleteUpgrade(hRequest, 0);
		hRequest = NULL; // WinHttpWebSocketCompleteUpgrade takes ownership
		if (!hWebSocket) {
			emitError("Failed to complete WebSocket upgrade.");
			return false;
		}

		if (onOpen) onOpen();

		running = true;
		recvThread = std::thread([this]() { receiveLoop(); });

		return true;
	}

	bool WebSocketClient::send(const std::string& message)
	{
		if (!hWebSocket) return false;
		DWORD res = WinHttpWebSocketSend(hWebSocket,
			WINHTTP_WEB_SOCKET_UTF8_MESSAGE_BUFFER_TYPE,
			(PVOID)message.c_str(), (DWORD)message.length());
		return res == NO_ERROR;
	}

	void WebSocketClient::close()
	{
		if (!running) return;

		running = false;

		if (hWebSocket) {
			WinHttpWebSocketClose(hWebSocket,
				WINHTTP_WEB_SOCKET_SUCCESS_CLOSE_STATUS, NULL, 0);
			WinHttpWebSocketShutdown(hWebSocket, WINHTTP_WEB_SOCKET_SUCCESS_CLOSE_STATUS);
		}

		if (recvThread.joinable()) {
			recvThread.join();
		}

		if (hWebSocket) WinHttpCloseHandle(hWebSocket);
		if (hRequest) WinHttpCloseHandle(hRequest);
		if (hConnect) WinHttpCloseHandle(hConnect);
		if (hSession) WinHttpCloseHandle(hSession);

		hWebSocket = NULL;
		hRequest = NULL;
		hConnect = NULL;
		hSession = NULL;

		if (onClose) onClose();
	}
}
