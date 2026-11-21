#pragma once

#ifndef _PROXY_SERVER_
#define _PROXY_SERVER_

#define _CRT_SECURE_NO_WARNINGS 1

// Avoid windows.h - ssl.h conflicts
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX

#include <windows.h>
#include <iostream>
#include <winsock2.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <string>
#include <sstream>
#include <ws2tcpip.h>

#include <chrono>
#include <thread>
#include <ctime>  // Import the ctime library

#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "libssl.lib")


#define PROXY_PORT_BY_DEFAULT 443
#define TARGET_PORT_BY_DEFAULT 7000+PROXY_PORT_BY_DEFAULT
#define BUFFER_SIZE 4096

extern const char* pathHTTPS;
extern const char* fileNameHTTPS;


class ProxyServer
{
public:
	ProxyServer();
	ProxyServer(int proxyServerPort, int targetServerPort, std::string targetServerUrl);

	~ProxyServer();

	std::string strSendGetRequest(SOCKET targetSocket, const std::string method, const std::string& path);
	std::string strSendPostRequest(SOCKET targetSocket, const std::string method, const std::string& path, const std::string& body);
	std::string strSendPutRequest(SOCKET targetSocket, const std::string method, const std::string& path, const std::string& body);

	std::string strForwardRequestToTargetServer(const std::string& method, const std::string& path, const std::string& body);
	void vProcessHttpsRequest(SSL* ssl);
	SSL_CTX* SSLInit();

	void WriteReportIntoFile(const char* pathHTTPS, const char* fileNameHTTPS, const char* mes);
	std::string GetCurrentDateTime();

	int iRun();

private:
	int proxyServerPort;

	int targetServerPort;
	std::string targetServerHost;

	const std::string iniFilePath = "options.ini";
};


#endif





