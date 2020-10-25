#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include <memory>
#include <functional>

class serverImp {
public:
    /* TCP Implementation */
    void setupTcp(int port);
    void closeTcp();
    void sendTcp(const char* buf);
    void recvTcp(char* buf, int len);
    void acceptConnection();

    /* UDP Implementation */
    void setupUdp(int port);
    void closeUdp();
    void sendUdp(const char* buf);
    void recvUdp(char* buf, int len);

private:
    int _port;
    int _serverSck;
    int _clientSck = -1;
    sockaddr_storage _srcAddr = {0};
};

class Server {
public:
    virtual ~Server() {};

public:
    virtual void send(const char* buf)       = 0;
    virtual void recv(char* buf, int len)    = 0; 
    virtual void run()                       = 0;
}; 

class TCPServer : public Server {
public:
    TCPServer(int port, std::function<void(char* buf, int len)> func);
    ~TCPServer();
    
    TCPServer(const TCPServer&)            = delete;
    TCPServer& operator=(const TCPServer&) = delete;

    TCPServer(TCPServer&&)            = delete;
    TCPServer& operator=(TCPServer&&) = delete;

    void send(const char* buf)    override final;
    void recv(char* buf, int len) override final; 
    void run()                    override final;

private:
    void accept();

private:
    serverImp _imp;
    std::function<void(char* buf, int len)> _func;
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
    void send(const char* buf)    override final;
    void recv(char* buf, int len) override final; 
    void run()                    override final;

private:
    serverImp _imp;
    std::function<void(char* buf, int len)> _func;
};