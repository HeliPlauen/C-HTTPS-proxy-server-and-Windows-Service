# A HTTPS proxy-server / Windows Service.

## Common ifrormation:

- proxy-server works as a Windows service;
- service name is **WinServ**;
- created using C++;
- target server URL is in the options.ini file;
- target server should work on the port taken from options.ini, currently in options.ini file there is 8888, by default - 8889;
- proxy server always works on the port 8200;
- SLL certificate and key required, currently uses self-signed certificate;
- both - HTTPS proxy-server and target server work on localhost, but target server may work anywhere.

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

- **Install**: sc create WinServ binPath="<FULL_!!!!_PATH_!!!!_to__proxy_server_dir>\WinServiceHttpsPROXYserver.exe"; 
- **Start**: sc start WinServ;
- **Stop**: sc stop WinServ;
- **Delete**: sc delete WinServ.

---

## III. How to start

1. If exe does not exist in 'exe' folder - you have to compile it, and then put it into 'exe' folder.
2. Be sure that OnenSSL is installed and exists in ths location "C:\Program Files\OpenSSL-Win64\include".
3. If **WinServ** is not installed as a Service - install it (watch chapter II).
4. Run **WinServ** (watch chapter II).

---

## IV. How to test

* run service using chapter III instructions; 
* run any server on the port equal to access_port fron options.ini (currentry it works on the port 8888) - you may use Service.exe from test;
* run cmd-command **curl -k https://127.0.0.1:8200**.
