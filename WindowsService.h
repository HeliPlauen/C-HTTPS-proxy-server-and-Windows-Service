#pragma once

#ifndef WINDOWS_SERVICE_CLASS_
#define WINDOWS_SERVICE_CLASS_


#define _CRT_SECURE_NO_WARNINGS 1 

#include "ProxyServer.h"

#include <windows.h>
#include <iostream>
#include <string>
#include <tchar.h>

#include <ctime>  // Import the ctime library


extern const char* path;
extern const char* fileName;


class WindowsService {
public:
    WindowsService(const TCHAR* serviceName, ProxyServer HTTPServ);
    ~WindowsService();

    void Install(TCHAR* argv[]);
    void Uninstall();
    void Start();
    void Stop();
    void Run();

    void WriteReportIntoFile(const char* path, const char* fileName, const char* mes);
    std::string GetCurrentDateTime();

private:
    static void WINAPI ServiceMain(DWORD argc, LPWSTR* argv);
    static void WINAPI ServiceCtrlHandler(DWORD ctrlCode);
    BOOL SetServiceStatusInct(DWORD currentState, DWORD win32ExitCode = NO_ERROR, DWORD waitHint = 0);

    static WindowsService* s_instance;    // Singleton instance
    ProxyServer m_HTTPServ;               // PROXY SERVER
    SERVICE_STATUS m_serviceStatus;
    SERVICE_STATUS_HANDLE m_serviceStatusHandle;
    const TCHAR* m_serviceName;

    const wchar_t* chServiceFullName = L"HTTPS Server on port 8200";

    const char* chBotServicePath = "options.ini";
};


#endif

