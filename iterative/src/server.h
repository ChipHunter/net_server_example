#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include <memory>
#include <functional>

class serverImp {
/* TCP Implementation */
public:
    void setupTcp(int port, std::function<void(char*, int)> func);
    void closeTcp();
    void runTcp();

private:
    void sendTcp(const char* buf);
    void recvTcp(char* buf, int len);
    void acceptConnection();
    
/* UDP Implementation */
public:
    void setupUdp(int port, std::function<void(char*, int)> func);
    void closeUdp();
    void runUdp();

private:
    void sendUdp(const char* buf);
    void recvUdp(char* buf, int len);

private:
    int _port = -1;
    int _serverSck = -1;
    int _clientSck = -1;
    sockaddr_storage _srcAddr = {0};
    std::function<void(char*, int)> _func;
};

class Server {
public:
    virtual ~Server() {};

public:
    virtual void run() = 0;
}; 

class TCPServer : public Server {
public:
    TCPServer(int port, std::function<void(char* buf, int len)> func);
    ~TCPServer();
    
    TCPServer(const TCPServer&)            = delete;
    TCPServer& operator=(const TCPServer&) = delete;

    TCPServer(TCPServer&&)            = delete;
    TCPServer& operator=(TCPServer&&) = delete;

    void run() override final;

private:
    serverImp _imp;
};

class UDPServer : public Server {
public:
    UDPServer(int port, std::function<void(char*, int)> func);
    ~UDPServer();
    
    UDPServer(const UDPServer&)            = delete;
    UDPServer& operator=(const UDPServer&) = delete;

    UDPServer(UDPServer&&)            = delete;
    UDPServer& operator=(UDPServer&&) = delete;

public:
    void run() override final;

private:
    serverImp _imp;
};