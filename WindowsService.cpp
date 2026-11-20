#include "WindowsService.h"


// Define them here !!!
const char* path = "C:\\BotServiceLogs\\SERVER_PORT_8200";
const char* fileName = "REPORT_SERVICE.txt";


WindowsService* WindowsService::s_instance = nullptr;


WindowsService::WindowsService(const TCHAR* serviceName, ProxyServer HTTPServ) : m_serviceName(serviceName), m_HTTPServ(HTTPServ), m_serviceStatusHandle(nullptr)
{
    s_instance = this;
    m_serviceStatus = { 0 };
    m_serviceStatus.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
    m_serviceStatus.dwControlsAccepted = SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_SHUTDOWN;

    std::cout << "CONSTRUCTOR SERVICE" << std::endl;
}


WindowsService::~WindowsService() {
    std::cout << "DESTRUCTOR SERVICE" << std::endl;
}


void WINAPI WindowsService::ServiceMain(DWORD argc, LPWSTR* argv) {
    BOOL res;
    int result;
    std::string message;

    s_instance->WriteReportIntoFile(path, fileName, "ServiceMain Start");

    s_instance->m_serviceStatusHandle = RegisterServiceCtrlHandler(s_instance->m_serviceName, ServiceCtrlHandler);
    if (s_instance->m_serviceStatusHandle == nullptr) {
        return;
    }

    // Initialize service status
    ZeroMemory(&(s_instance->m_serviceStatus), sizeof(s_instance->m_serviceStatus));
    s_instance->m_serviceStatus.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
    s_instance->m_serviceStatus.dwCurrentState = SERVICE_START_PENDING;
    s_instance->m_serviceStatus.dwControlsAccepted = SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_SHUTDOWN;
    s_instance->m_serviceStatus.dwWin32ExitCode = 0;
    s_instance->m_serviceStatus.dwServiceSpecificExitCode = 0;
    s_instance->m_serviceStatus.dwCheckPoint = 0;
    s_instance->m_serviceStatus.dwWaitHint = 0;

    // Set the status to running
    res = s_instance->SetServiceStatusInct(SERVICE_RUNNING, 0, 0);
    if (res == FALSE) { 
        s_instance->WriteReportIntoFile(path, fileName, "ServiceMain: SetServiceStatus returned error - RUN!!!");
        return;
    }

    // The main service loop - START PROXY SERVER ON PORT 8200 FOR TARGET SERVER ON PORT (Default - 7030)
    message = "HTTPS SERVER ON PORT 8200 STARTED !!!" + s_instance->GetCurrentDateTime();
    s_instance->WriteReportIntoFile(path, fileName, message.c_str());
    result = s_instance->m_HTTPServ.iRun();
    if (result != 0) {
        message = "HTTPS Server encountered an error: " + std::to_string(result);
        s_instance->WriteReportIntoFile(path, fileName, message.c_str());
    }

    res = s_instance->SetServiceStatusInct(SERVICE_STOPPED, 0, 0);
    if (res == FALSE) {
        s_instance->WriteReportIntoFile(path, fileName, "ServiceMain: SetServiceStatus returned error - STOP!!!");
        return;
    }
}


void WINAPI WindowsService::ServiceCtrlHandler(DWORD ctrlCode) {
    BOOL res;
    std::string message;

    switch (ctrlCode) {
        if (s_instance->m_serviceStatus.dwCurrentState != SERVICE_RUNNING) {
            break;
        }

        case SERVICE_CONTROL_STOP: {
            res = s_instance->SetServiceStatusInct(SERVICE_STOP_PENDING, 0, 0);
            if (res == FALSE) {
                s_instance->WriteReportIntoFile(path, fileName, "ServiceCtrlHandler: SetServiceStatus returned error - STOP PENDING!!!");
                return;
            }

            // Perform any cleanup tasks here

            s_instance->SetServiceStatusInct(SERVICE_STOPPED, 0, 0);
            if (res == FALSE) {
                s_instance->WriteReportIntoFile(path, fileName, "ServiceCtrlHandler: SetServiceStatus returned error - STOP!!!");
                return;
            }

            message = "HTTPS SERVER ON PORT 8200 STOPED !!!" + s_instance->GetCurrentDateTime();
            s_instance->WriteReportIntoFile(path, fileName, message.c_str());
            break;
        }
        default: {
            break;
        }
    }
}


BOOL WindowsService::SetServiceStatusInct(DWORD currentState, DWORD win32ExitCode, DWORD waitHint) {
    this->m_serviceStatus.dwCurrentState = currentState;
    this->m_serviceStatus.dwWin32ExitCode = win32ExitCode;
    this->m_serviceStatus.dwWaitHint = waitHint;
    return SetServiceStatus(this->m_serviceStatusHandle, &(this->m_serviceStatus));
}



void WindowsService::Install(TCHAR* argv[]) {
    SC_HANDLE hSCManager;
    SC_HANDLE hService;
    std::string message;
    DWORD dwGetModuleFileName;
    TCHAR szPath[MAX_PATH];

    // GetModuleFileName Get the Executable file for CSM
    dwGetModuleFileName = GetModuleFileNameW(NULL, szPath, MAX_PATH);
    if (dwGetModuleFileName == 0) {
        message = "INSTALL: Path to EXE file was NOT gotten: " + std::to_string(GetLastError());
        this->WriteReportIntoFile(path, fileName, message.c_str());
    }
    else {
        this->WriteReportIntoFile(path, fileName, "INSTALL: Path to EXE file was gotten SUCCESSFULLY !!!");
    }

    hSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_CREATE_SERVICE);
    if (hSCManager == NULL) {
        message = "INSTALL: OpenSCManager failed, error code: " + std::to_string(GetLastError());
        this->WriteReportIntoFile(path, fileName, message.c_str());
        return;
    }

    hService = CreateService(
        hSCManager,                         // SCM database
        this->m_serviceName,                // Name of service 
        this->chServiceFullName,              // Service name to display
        SERVICE_ALL_ACCESS,                 // Desired access 
        SERVICE_WIN32_OWN_PROCESS,          // Service type 
        SERVICE_DEMAND_START,               // Start type 
        SERVICE_ERROR_NORMAL,               // Error control type 
        szPath,                             // Path to service's binary 
        NULL,                               // No load ordering group 
        NULL,                               // No tag identifier 
        NULL,                               // No dependencies 
        NULL,                               // LocalSystem account 
        NULL);                              // No password 

    if (hService == NULL) {
        message = "INSTALL: CreateService failed, error code: " + std::to_string(GetLastError());
        this->WriteReportIntoFile(path, fileName, message.c_str());
    }
    else {
        message = "INSTALL: Service installed successfully." + this->GetCurrentDateTime();
        this->WriteReportIntoFile(path, fileName, message.c_str());
        CloseServiceHandle(hService);
    }

    CloseServiceHandle(hSCManager);
}


void WindowsService::Start() {
    SC_HANDLE hSCManager;
    SC_HANDLE hService;
    std::string message;

    hSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_CONNECT);
    if (hSCManager == NULL) {
        message = "RUN: OpenSCManager failed, error code: " + std::to_string(GetLastError());
        this->WriteReportIntoFile(path, fileName, message.c_str());
        return;
    }

    hService = OpenService(hSCManager, this->m_serviceName, SERVICE_START);
    if (hService == NULL) {
        message = "RUN: OpenService failed, error code: " + std::to_string(GetLastError());
        this->WriteReportIntoFile(path, fileName, message.c_str());
    }
    else {
        if (::StartService(hService, 0, NULL)) {
            message = "RUN: Service started successfully." + this->GetCurrentDateTime();
            this->WriteReportIntoFile(path, fileName, message.c_str());
        }
        else {
            message = "RUN: StartService failed, error code: " + std::to_string(GetLastError());
            this->WriteReportIntoFile(path, fileName, message.c_str());
        }
        CloseServiceHandle(hService);
    }

    CloseServiceHandle(hSCManager);
}


void WindowsService::Stop() {
    SC_HANDLE hSCManager;
    SC_HANDLE hService;
    std::string message;

    hSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_CONNECT);
    if (hSCManager == NULL) {
        message = "STOP: OpenSCManager failed, error code: " + std::to_string(GetLastError());
        this->WriteReportIntoFile(path, fileName, message.c_str());
        return;
    }

    hService = OpenService(hSCManager, this->m_serviceName, SERVICE_STOP);
    if (hService == NULL) {
        message = "STOP: OpenService failed, error code: " + std::to_string(GetLastError());
        this->WriteReportIntoFile(path, fileName, message.c_str());
    }
    else {
        SERVICE_STATUS serviceStatus;
        if (ControlService(hService, SERVICE_CONTROL_STOP, &serviceStatus)) {
            message = "STOP: Service stopped successfully." + this->GetCurrentDateTime();
            this->WriteReportIntoFile(path, fileName, message.c_str());
        }
        else {
            message = "STOP: Stop Service failed, error code: " + std::to_string(GetLastError());
            this->WriteReportIntoFile(path, fileName, message.c_str());
        }
        CloseServiceHandle(hService);
    }

    CloseServiceHandle(hSCManager);
}


void WindowsService::Uninstall() {
    SC_HANDLE hSCManager;
    SC_HANDLE hService;
    std::string message;

    hSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_CONNECT);
    if (hSCManager == NULL) {
        message = "UNINSTALL: OpenSCManager failed, error code: " + std::to_string(GetLastError());
        this->WriteReportIntoFile(path, fileName, message.c_str());
        return;
    }

    hService = OpenService(hSCManager, this->m_serviceName, DELETE);
    if (hService == NULL) {
        message = "UNINSTALL: OpenService failed, error code: " + std::to_string(GetLastError());
        this->WriteReportIntoFile(path, fileName, message.c_str());
    }
    else {
        if (DeleteService(hService)) {
            message = "UNINSTALL: Service uninstalled successfully." + this->GetCurrentDateTime();
            this->WriteReportIntoFile(path, fileName, message.c_str());
        }
        else {
            message = "UNINSTALL: DeleteService failed, error code: " + std::to_string(GetLastError());
            this->WriteReportIntoFile(path, fileName, message.c_str());
        }
        CloseServiceHandle(hService);
    }

    CloseServiceHandle(hSCManager);
}


void WindowsService::Run() {
    std::string message;

    SERVICE_TABLE_ENTRY serviceTable[] = {
        { const_cast<TCHAR*>(this->m_serviceName), ServiceMain },
        { nullptr, nullptr }
    };

    if (!StartServiceCtrlDispatcher(serviceTable)) {
        message = "RUN: StartServiceCtrlDispatcher failed, error code: " + std::to_string(GetLastError()) + this->GetCurrentDateTime();
        this->WriteReportIntoFile(path, fileName, message.c_str());
    }
}


void WindowsService::WriteReportIntoFile(const char* path, const char* fileName, const char* mes) {

    // Create the directory if it doesn't exist
    if (!CreateDirectoryA(path, NULL)) {
        if (GetLastError() != ERROR_ALREADY_EXISTS) {
            printf("Failed to create directory! Error: %lu\n", GetLastError());
            return;
        }
    }

    std::string fullPath(path);
    fullPath += "\\";
    fullPath += fileName;

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


std::string WindowsService::GetCurrentDateTime() {
    std::string msg = ". DATE AND  TIME: ";

    // Get the timestamp for the current date and time
    time_t timestamp;
    time(&timestamp);
    auto datetime = ctime(&timestamp);   

    msg += datetime;
    return msg;
}