# A HTTPS proxy-server / Windows Service.

## Common ifrormation:

- proxy-server works as a Windows service;
- service name is **WinServ**;
- created using C++;
- target server should work on the port taken from options.ini, by default - 7031;
- should work with a Windows proxy-server, both should run on the port = target server should + 1170;
- SLL certificate and key required, currently uses self-signed certificate;
- start Windows proxy-server before WinServ service start;
- both - HTTPS proxy-server and target server should work on localhost (such limits may be solved easilly).

---

## How to install, start, stop, delete.

### I. CMD command

**Install**
- cd <proxy_server_dir_path>
- WinServiceHttpsPROXYserver.exe install

**Start**
- cd <proxy_server_dir_path>
- WinServiceHttpsPROXYserver.exe start

**Stop**
- cd <proxy_server_dir_path>
- WinServiceHttpsPROXYserver.exe stop

**uninstall**
- cd <proxy_server_dir_path>
- WinServiceHttpsPROXYserver.exe uninstall

### II. Using CS

- **Install**: sc create WinServ binPath="<proxy_server_dir_path>\WinServiceHttpsPROXYserver.exe"; 
- **Start**: sc start WinServ;
- **Stop**: sc stop WinServ;
- **Delete**: sc delete WinServ.