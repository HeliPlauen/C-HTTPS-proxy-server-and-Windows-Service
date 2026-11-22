# HTTPS Reverse Proxy Server (Windows Service)

A lightweight HTTPS reverse proxy server written in C++ using **WinSock** and **OpenSSL**, designed to run as a background **Windows Service**.

## Overview

- **Architecture:** Acts as an HTTPS reverse proxy (`Client -> Proxy :8200 -> Target Server :8888`).
- **Service Name:** `WinServ`
- **Tech Stack:** C++, WinSock2, OpenSSL, Windows Service API.
- **Configuration (`options.ini`):**
  - Target server host/URL and port are configured via `options.ini` (currently set to `127.0.0.1:8888`, default fallback port is `8889`).
  - The proxy listens on port `8200` by default.
- **SSL/TLS:** Requires an SSL certificate and private key (`ssl\server.crt` and `ssl\server.key`). Currently configured with a self-signed certificate for local testing.
- **Deployment:** Both the proxy and target server run on `localhost` by default, though the target server can be hosted on any remote IP address.

---

## Prerequisites & Setup

1. **OpenSSL:** Ensure OpenSSL is installed at `C:\Program Files\OpenSSL-Win64\include` (or update the include/library paths in your project settings).
2. **Build:** If the binary is not present in the `exe` folder, compile the project in Visual Studio and place `WinServiceHttpsPROXYserver.exe` into the `exe` directory alongside the `ssl` folder and `options.ini`.

---

## Service Management

> **Note:** Run Command Prompt (`cmd`) or PowerShell as **Administrator**.

### Option 1: Built-in CLI Commands

Navigate to the directory containing the executable (`cd <proxy_server_dir_path>`) and run:

```cmd
:: Install the service
WinServiceHttpsPROXYserver.exe install
```
```cmd
:: Start the service
WinServiceHttpsPROXYserver.exe start
```
```cmd
:: Stop the service
WinServiceHttpsPROXYserver.exe stop
```
```cmd
:: Uninstall the service
WinServiceHttpsPROXYserver.exe uninstall
```

### Option 2: Using Windows Service Control (`sc`)

```cmd
:: Install (replace with the actual full path to the executable)
sc create WinServ binPath= "C:\(\full\path\to\WinServiceHttpsPROXYserver.\)exe"
```
```cmd
:: Start
sc start WinServ
```
```cmd
:: Stop
sc stop WinServ
```
```cmd
:: Delete
sc delete WinServ
```

---

## How to Test

1. Install and start the `WinServ` service using one of the methods above.
2. Start a target HTTPS server listening on the port specified in `options.ini` (e.g., `8888`). You can use the provided `Service.exe` from the `test` directory.
3. Send a test request via `curl` (use `-k` to allow the self-signed certificate):

```cmd
curl -k https://127.0.0.1:8200
```
