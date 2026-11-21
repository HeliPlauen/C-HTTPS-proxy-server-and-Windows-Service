#include "ProxyServer.h"
#include "IniParcer.h"

#include <typeinfo>


// Define them here !!!
const char* pathHTTPS = "SERVER_PORT_8200";
const char* fileNameHTTPS = "REPORT_HTTPS.txt";
const char* fileErrNameHTTPS = "ERR_REPORT_HTTPS.txt";


ProxyServer::ProxyServer() {
    std::cout << "CONSTRUCTOR PROXY-SERVER-1" << std::endl;

    // Set current dir !!!
    wchar_t exePath[MAX_PATH];
    if (GetModuleFileNameW(NULL, exePath, MAX_PATH) > 0) {
        std::wstring dirPath(exePath);
        size_t lastSlash = dirPath.find_last_of(L"\\/");
        if (lastSlash != std::wstring::npos) {
            dirPath = dirPath.substr(0, lastSlash);
            SetCurrentDirectoryW(dirPath.c_str());
        }
    }

    IniParcer parcer = IniParcer(this->iniFilePath);
    auto data = parcer.Run();

    this->targetServerHost = data.first;
    std::cout << "Target server URL: " << this->targetServerHost << std::endl;

    this->targetServerPort = data.second;
    std::cout << "Target server port: " << this->targetServerPort << std::endl;

    this->proxyServerPort = 8200;
    std::cout << "Access port: " << this->proxyServerPort << std::endl;    
};


ProxyServer::ProxyServer(int proxyServerPort, int targetServerPort, std::string targetServerUrl) {
    std::cout << "CONSTRUCTOR PROXY-SERVER-2" << std::endl;

    // Set current dir !!!
    wchar_t exePath[MAX_PATH];
    if (GetModuleFileNameW(NULL, exePath, MAX_PATH) > 0) {
        std::wstring dirPath(exePath);
        size_t lastSlash = dirPath.find_last_of(L"\\/");
        if (lastSlash != std::wstring::npos) {
            dirPath = dirPath.substr(0, lastSlash);
            SetCurrentDirectoryW(dirPath.c_str());
        }
    }
        
    this->targetServerPort = targetServerPort;
    this->targetServerHost = targetServerUrl;
    this->proxyServerPort = proxyServerPort;

    std::cout << "Target server URL: " << this->targetServerHost << std::endl;
    std::cout << "Access port: " << this->proxyServerPort << std::endl;
    std::cout << "Target server port: " << this->targetServerPort << std::endl;    
};


ProxyServer::~ProxyServer() {
    std::cout << "DESTRUCTOR PROXY-SERVER" << std::endl;
};


std::string ProxyServer::strSendGetRequest(SOCKET targetSocket, const std::string method, const std::string& path) {
    int bytesReceived;
    std::string httpRequest;
    std::string httpResponse;
    char buffer[BUFFER_SIZE];
    std::string mess;

    httpRequest =
        httpRequest =
        method + " " + path + " HTTP/1.1\r\n"
        "Host: " + this->targetServerHost + ":" + std::to_string(this->targetServerPort) + "\r\n"        
        //"Content-Type: application/x-www-form-urlencoded\r\n\r\n"
        "Content-Type: text/html\r\n"
        "Connection: close\r\n\r\n";

    this->WriteReportIntoFile(pathHTTPS, fileNameHTTPS, httpRequest.c_str());

    send(targetSocket, httpRequest.c_str(), static_cast<int>(httpRequest.length()), 0);

    mess = "WE HAVE SENT GET-REQUEST TO THE TARGET SERVER: \n" + httpRequest + this->GetCurrentDateTime();
    this->WriteReportIntoFile(pathHTTPS, fileNameHTTPS, mess.c_str());

    // Add a small delay
    std::this_thread::sleep_for(std::chrono::milliseconds(50));

    // Receive the response from the target server
    bytesReceived = recv(targetSocket, buffer, BUFFER_SIZE, 0);
    httpResponse = std::string(buffer, bytesReceived);

    mess = "WE HAVE RECEIVED FROM THE TARGET SERVER: \n" + httpResponse + this->GetCurrentDateTime();
    this->WriteReportIntoFile(pathHTTPS, fileNameHTTPS, mess.c_str());

    return httpResponse;
}


std::string ProxyServer::strSendPostRequest(SOCKET targetSocket, const std::string method, const std::string& path, const std::string& body) {
    int bytesReceived;
    std::string httpRequest;
    std::string httpResponse;
    char buffer[BUFFER_SIZE];
    std::string mess;

    httpRequest =
        method + " " + path + " HTTP/1.1\r\n"
        "Host: " + this->targetServerHost + ":" + std::to_string(this->targetServerPort) + "\r\n"
        "Content-Type: text/html\r\n"
        "Content-Length: " + std::to_string(static_cast<int>(body.length())) + "\r\n"
        "\r\n" + body + 
        "Connection: close\r\n\r\n";

    send(targetSocket, httpRequest.c_str(), static_cast<int>(httpRequest.length()), 0);

    mess = "WE HAVE SENT POST-REQUEST TO THE TARGET SERVER: \n" + httpRequest + this->GetCurrentDateTime();
    this->WriteReportIntoFile(pathHTTPS, fileNameHTTPS, mess.c_str());

    // Add a small delay
    std::this_thread::sleep_for(std::chrono::milliseconds(50));

    // Receive the response from the target server
    bytesReceived = recv(targetSocket, buffer, BUFFER_SIZE, 0);
    httpResponse = std::string(buffer, bytesReceived);

    mess = "WE HAVE RECEIVED FROM THE TARGET SERVER: \n" + httpResponse + this->GetCurrentDateTime();
    this->WriteReportIntoFile(pathHTTPS, fileNameHTTPS, mess.c_str());

    return httpResponse;
}


std::string ProxyServer::strSendPutRequest(SOCKET targetSocket, const std::string method, const std::string& path, const std::string& body) {
    int bytesReceived;
    std::string httpRequest;
    std::string httpResponse;
    char buffer[BUFFER_SIZE];
    std::string mess;

    httpRequest =
        method + " " + path + " HTTP/1.1\r\n"
        "Host: " + this->targetServerHost + ":" + std::to_string(this->targetServerPort) + "\r\n"
        "Content-Type: text/html\r\n"
        "Content-Length: " + std::to_string(static_cast<int>(body.length())) + "\r\n"
        "\r\n" + body +
        "Connection: close\r\n\r\n";

    send(targetSocket, httpRequest.c_str(), static_cast<int>(httpRequest.length()), 0);

    mess = "WE HAVE SENT PUT-REQUEST TO THE TARGET SERVER: \n" + httpRequest + this->GetCurrentDateTime();
    this->WriteReportIntoFile(pathHTTPS, fileNameHTTPS, mess.c_str());

    // Add a small delay
    std::this_thread::sleep_for(std::chrono::milliseconds(50));

    // Receive the response from the target server
    bytesReceived = recv(targetSocket, buffer, BUFFER_SIZE, 0);
    httpResponse = std::string(buffer, bytesReceived);

    mess = "WE HAVE RECEIVED FROM THE TARGET SERVER: \n" + httpResponse + this->GetCurrentDateTime();
    this->WriteReportIntoFile(pathHTTPS, fileNameHTTPS, mess.c_str());

    return httpResponse;
}


// Function to send an HTTP request to the target server and receive the response
std::string ProxyServer::strForwardRequestToTargetServer(const std::string& method, const std::string& path, const std::string& body) {
    SOCKET targetSocket;
    struct sockaddr_in targetAddr;
    std::string httpResponse;

    int res = 0;
    std::string err_mess;

    // Create a socket to connect to the target server
    if ((targetSocket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        std::string err_mess = "Socket creation failed: " + std::to_string(WSAGetLastError());
        this->WriteReportIntoFile(pathHTTPS, fileErrNameHTTPS, err_mess.c_str());

        return "";
    }

    targetAddr.sin_family = AF_INET;
    targetAddr.sin_port = htons(this->targetServerPort);

    const char* url = this->targetServerHost.c_str();
    inet_pton(AF_INET, url, &targetAddr.sin_addr);

    // 1. Створюємо клієнтський SSL-контекст (можна зробити один раз при старті проксі)
    SSL_CTX* clientCtx = SSL_CTX_new(TLS_client_method());
    SSL_CTX_set_verify(clientCtx, SSL_VERIFY_NONE, nullptr); // Ігноруємо самопідписаний сертифікат сервера

    // 2. Підключаємо SSL до вже відкритого сокета targetSocket
    SSL* targetSsl = SSL_new(clientCtx);
    SSL_set_fd(targetSsl, static_cast<int>(targetSocket));

    if (SSL_connect(targetSsl) <= 0) {

        // Помилка TLS handshake із сервером 8888
        ERR_print_errors_fp(stderr);
        SSL_free(targetSsl);
        SSL_CTX_free(clientCtx);

        closesocket(targetSocket);
        return "";
    }

    // !!!
    // Connect to the target server
    res = connect(targetSocket, (struct sockaddr*)&targetAddr, sizeof(targetAddr));
    if (res < 0) {
        err_mess = "Connection to target server failed: " + std::to_string(WSAGetLastError());
        this->WriteReportIntoFile(pathHTTPS, fileErrNameHTTPS, err_mess.c_str());

        err_mess = "[" + this->targetServerHost + "]";
        this->WriteReportIntoFile(pathHTTPS, fileErrNameHTTPS, err_mess.c_str());

        err_mess = "[" + std::to_string(this->targetServerPort) + " " + typeid(this->targetServerPort).name() + "]";
        this->WriteReportIntoFile(pathHTTPS, fileErrNameHTTPS, err_mess.c_str());

        closesocket(targetSocket);
        return "";
    }

    if (method == "GET") {
        this->WriteReportIntoFile(pathHTTPS, fileNameHTTPS, "HERE-5");

        httpResponse = this->strSendGetRequest(targetSocket, method, path);
    }
    else if (method == "POST") {
        httpResponse = this->strSendPostRequest(targetSocket, method, path, body);
    }
    else if (method == "PUT") {
        httpResponse = this->strSendPutRequest(targetSocket, method, path, body);
    }
    else {
        httpResponse =
            "HTTP/1.1 405 Method Not Allowed\r\n"
            "Content-Length: 0\r\n"
            "\r\n";
    }

    // Close the target server connection
    closesocket(targetSocket);

    return httpResponse;
}


void ProxyServer::vProcessHttpsRequest(SSL* ssl) {
    char buffer[BUFFER_SIZE];
    char* buffer_2 = nullptr;
    std::string method, path, httpVersion;
    std::string headers;
    std::string body;
    std::string response;
    int contentLength = 0;
    int bytesRead;
    int bytesReceived = SSL_read(ssl, buffer, BUFFER_SIZE);

    if (bytesReceived > 0) {
        buffer[bytesReceived] = '\0';
        //std::cout << "Received HTTPS request: \n" << buffer << std::endl;
    }
    std::istringstream requestStream(buffer);

    // Parse the request line
    std::getline(requestStream, method, ' ');
    std::getline(requestStream, path, ' ');
    std::getline(requestStream, httpVersion);

    // Parse headers
    while (std::getline(requestStream, headers) && headers != "\r") {
        headers += "\n";

        // Check for Content-Length header
        if (headers.find("Content-Length:") != std::string::npos) {
            contentLength = std::stoi(headers.substr(16));
        }
    }

    // Log the request details for debugging
    std::string method_log = "METHOD: " + method;
    this->WriteReportIntoFile(pathHTTPS, fileNameHTTPS, method_log.c_str());

    std::string path_log = "PATH: " + path;
    this->WriteReportIntoFile(pathHTTPS, fileNameHTTPS, path_log.c_str());

    std::string version_log = "HTTP VERSION: " + httpVersion;
    this->WriteReportIntoFile(pathHTTPS, fileNameHTTPS, version_log.c_str());

    std::string headers_log = "HEADERS: " + headers;
    this->WriteReportIntoFile(pathHTTPS, fileNameHTTPS, headers_log.c_str());

    // Read the body, handling different scenarios
    if (contentLength > 0) {

        // Try reading the body from the stream (works for Python requests)
        std::getline(requestStream, body, '\0');
        body.erase(0, body.find_first_not_of("\r\n"));

        if (static_cast<int>(body.length()) < contentLength) {

            // If the body is incomplete, read the remaining content via SSL_read (handles client case)
            buffer_2 = new char[contentLength + 1];
            bytesRead = SSL_read(ssl, buffer_2, contentLength - static_cast<int>(body.length()));
            if (bytesRead > 0) {
                buffer_2[bytesRead] = '\0';
                body += std::string(buffer_2);
            }
            delete[] buffer_2;
        }
    }
    else {

        // If no Content-Length, attempt to read any remaining data as body
        if (std::getline(requestStream, body)) {
            body.erase(0, body.find_first_not_of("\r\n"));
        }
    }

    // std::cout << "BODY:\n" << body << std::endl;

    // Forward the request to the target server and get the response
    response = this->strForwardRequestToTargetServer(method, path, body);

    // Send the response back to the client
    SSL_write(ssl, response.c_str(), static_cast<int>(response.length()));
}


SSL_CTX* ProxyServer::SSLInit() {
    char path[MAX_PATH];
    std::string fullSSLpathCert;
    std::string fullSSLpathKey;

    // Step 1: Initialize OpenSSL
    SSL_library_init();
    OpenSSL_add_all_algorithms();
    SSL_load_error_strings();
    const SSL_METHOD* method = SSLv23_server_method();
    SSL_CTX* ctx = SSL_CTX_new(method);
    if (!ctx) {
        this->WriteReportIntoFile(pathHTTPS, fileNameHTTPS, "SSL-ERROR: Unable to create SSL context.");
        ERR_print_errors_fp(stderr);
        return nullptr;
    }
    
    // Get my Servece EXE path
    if (GetModuleFileNameA(NULL, path, MAX_PATH) > 0) {
        std::string fullPath(path);

        // Remove the executable name from the path
        fullPath = fullPath.substr(0, fullPath.find_last_of("\\/"));
        this->WriteReportIntoFile(pathHTTPS, fileNameHTTPS, fullPath.c_str());

        fullSSLpathCert = fullPath;
        fullSSLpathKey = fullPath;
    }
    else {
        this->WriteReportIntoFile(pathHTTPS, fileNameHTTPS, "SSL-ERROR: Could not get Service EXE file path!");
        return nullptr;
    }       

    // Step 2: Load certificate and private key
    fullSSLpathCert += "\\ssl\\server.crt";
    if (SSL_CTX_use_certificate_file(ctx, fullSSLpathCert.c_str(), SSL_FILETYPE_PEM) <= 0) {
        ERR_print_errors_fp(stderr);
        this->WriteReportIntoFile(pathHTTPS, fileNameHTTPS, "SSL-ERROR: Problems with CERTIFICATE.CRT or it wasnot found.");
        return nullptr;
    }
    
    fullSSLpathKey += "\\ssl\\server.key";
    if (SSL_CTX_use_PrivateKey_file(ctx, fullSSLpathKey.c_str(), SSL_FILETYPE_PEM) <= 0) {
        ERR_print_errors_fp(stderr);
        this->WriteReportIntoFile(pathHTTPS, fileNameHTTPS, "SSL-ERROR: Problems with CERTIFICATE.KEY or it wasnot found.");
        return nullptr;
    }

    this->WriteReportIntoFile(pathHTTPS, fileNameHTTPS, "SSL: SSL context created successfully.");
    return ctx;
}


void ProxyServer::WriteReportIntoFile(const char* pathHTTPS, const char* fileNameHTTPS, const char* mes) {

    // Create the directory if it doesn't exist
    if (!CreateDirectoryA(pathHTTPS, NULL)) {
        if (GetLastError() != ERROR_ALREADY_EXISTS) {
            printf("Failed to create directory! Error: %lu\n", GetLastError());
            return;
        }
    }

    std::string fullPath(pathHTTPS);
    fullPath += "\\";
    fullPath += fileNameHTTPS;

    // Now proceed to open and write to the file
    FILE* fptr = NULL;
    fptr = fopen(fullPath.c_str(), "a+");
    if (fptr == NULL) {
        printf("The file was not opened!!\n");
        return;
    }

    fwrite(mes, strlen(mes), 1, fptr);
    fwrite("\n", sizeof(char), 1, fptr);
    fclose(fptr);
}


std::string ProxyServer::GetCurrentDateTime() {
    std::string msg = ". DATE AND  TIME: ";

    // Get the timestamp for the current date and time
    time_t timestamp;
    time(&timestamp);
    auto datetime = ctime(&timestamp);

    msg += datetime;
    return msg;
}


int ProxyServer::iRun() {
    WSADATA wsaData;
    SOCKET proxySocket, clientSocket;
    struct sockaddr_in proxyAddr, clientAddr;
    int clientAddrSize = sizeof(clientAddr);
    SSL_CTX* ctx = nullptr;
    std::string mess;

    // Step 1: Initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::string err_mess = "SSL initialization failed - 1." + std::to_string(WSAGetLastError());
        this->WriteReportIntoFile(pathHTTPS, fileErrNameHTTPS, err_mess.c_str());

        return 1;
    }

    // Step 2: Initialize OpenSSL
    ctx = this->SSLInit();
    if (ctx == nullptr) {
        std::string err_mess = "SSL initialization failed - 2." + std::to_string(WSAGetLastError());
        this->WriteReportIntoFile(pathHTTPS, fileErrNameHTTPS, err_mess.c_str());

        WSACleanup();
        return 2;
    }

    // Step 4: Create a socket
    if ((proxySocket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        std::string err_mess = "Socket creation failed: " + std::to_string(WSAGetLastError());
        this->WriteReportIntoFile(pathHTTPS, fileErrNameHTTPS, err_mess.c_str());

        WSACleanup();
        return 3;
    }

    // Step 5: Bind the socket to the network address and port
    proxyAddr.sin_family = AF_INET;
    proxyAddr.sin_addr.s_addr = INADDR_ANY;
    proxyAddr.sin_port = htons(this->proxyServerPort);

    if (bind(proxySocket, (struct sockaddr*)&proxyAddr, sizeof(proxyAddr)) == SOCKET_ERROR) {
        std::string err_mess = "Bind failed: " + std::to_string(WSAGetLastError());
        this->WriteReportIntoFile(pathHTTPS, fileErrNameHTTPS, err_mess.c_str());

        closesocket(proxySocket);
        WSACleanup();
        return 4;
    }

    // Step 6: Listen for incoming connections
    if (listen(proxySocket, 3) == SOCKET_ERROR) {
        std::string err_mess = "Listen failed: " + std::to_string(WSAGetLastError());
        this->WriteReportIntoFile(pathHTTPS, fileErrNameHTTPS, err_mess.c_str());

        closesocket(proxySocket);
        WSACleanup();
        return 5;
    }

    mess = "HTTPS PROXY SERVER IS LISTENING ON A PORT: " + std::to_string(this->proxyServerPort) + this->GetCurrentDateTime();
    this->WriteReportIntoFile(pathHTTPS, fileNameHTTPS, mess.c_str());

    mess = "HTTP TARGET SERVER HAS A PORT: " + std::to_string(this->targetServerPort) + this->GetCurrentDateTime();
    this->WriteReportIntoFile(pathHTTPS, fileNameHTTPS, mess.c_str());

    // Step 7: Accept and process incoming connections
    while (true) {
        clientSocket = accept(proxySocket, (struct sockaddr*)&clientAddr, &clientAddrSize);
        if (clientSocket == INVALID_SOCKET) {
            std::string err_mess = "Accept failed: " + std::to_string(WSAGetLastError());
            this->WriteReportIntoFile(pathHTTPS, fileErrNameHTTPS, err_mess.c_str());

            continue;
        }

        SSL* ssl = SSL_new(ctx);
        SSL_set_fd(ssl, clientSocket);

        if (SSL_accept(ssl) <= 0) {
            ERR_print_errors_fp(stderr);
        }
        else {
            this->vProcessHttpsRequest(ssl);
        }

        SSL_shutdown(ssl);
        SSL_free(ssl);
        closesocket(clientSocket);
    }

    // Step 8: Cleanup
    closesocket(proxySocket);
    SSL_CTX_free(ctx);
    WSACleanup();

    return 0;
}


