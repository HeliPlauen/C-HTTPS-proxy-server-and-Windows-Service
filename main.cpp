#include "WindowsService.h"
#include "ProxyServer.h"
#include "IniParcer.h"


// x = requests.get('https://127.0.0.1:8200', verify=False)
// x = requests.post('https://127.0.0.1:8200', json = { 'action':'unlock_service' }, verify=False)
// x = requests.put('https://127.0.0.1:8200', json = { 'action':'unlock_service' }, verify=False)
// x = requests.delete('https://127.0.0.1:8200', json = { 'action':'unlock_service' }, verify=False)


#define SERVICE_NAME TEXT("WinServ")


int _tmain(int argc, TCHAR* argv[]) {
    ProxyServer HTTPServ = ProxyServer();    
    WindowsService service(SERVICE_NAME, HTTPServ);

    if (argc > 1) {
        if (_tcscmp(argv[1], TEXT("install")) == 0) {
            service.Install(argv);
        }        
        else if (_tcscmp(argv[1], TEXT("start")) == 0) {
            service.Start();
        }
        else if (_tcscmp(argv[1], TEXT("stop")) == 0) {
            service.Stop();
        }
        else if (_tcscmp(argv[1], TEXT("uninstall")) == 0) {
            service.Uninstall();
        }
        else{
            return 1;
        }
    }
    else {
        service.Run();
    }

    return 0;
}

