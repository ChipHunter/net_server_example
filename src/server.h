#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <iostream>
#include <arpa/inet.h>
#include <unistd.h>
#include <thread>
#include <mutex>
#include <vector>
#include <queue>
#include <memory>

class serverImp {
public:
    /* TCP Implementation */
    void setupTcp(int port);
    void closeTcp();
    void sendTcp(const char* buf);
    void recvTcp(char* buf, int len);
    void acceptConnection();

    /* UDP Implementation */
   // void sendUdp(int sck, const char* buf);
   // void recvUdp(int sck, char* buf, int len);

private:
    int _port;
    int _serverSck;
    int _clientSck = -1;
};

class Server {
public:
    virtual ~Server() {};

public:
    virtual void send(const char* buf)       = 0;
    virtual void recv(char* buf, int len)    = 0; 
    virtual void handle_request()            = 0;
}; 

class TCPServer : public Server {
public:
    TCPServer(int port);
    ~TCPServer();
    
    TCPServer(const TCPServer&)            = delete;
    TCPServer& operator=(const TCPServer&) = delete;

    TCPServer(TCPServer&&)            = delete;
    TCPServer& operator=(TCPServer&&) = delete;

public:
    void send(const char* buf)    override final;
    void recv(char* buf, int len) override final; 
    void handle_request()         override final;
    void accept();

private:
    serverImp _imp;
};

/*class UDPServer : public Server {
public:
    UDPServer();
    ~UDPServer();
    
    UDPServer(const UDPServer&)            = delete;
    UDPServer& operator=(const UDPServer&) = delete;

    UDPServer(UDPServer&&)            = delete;
    UDPServer& operator=(UDPServer&&) = delete;

public:
    void send(const char* buf)       override final;
    void recv(char* buf, int len)    override final; 
    void handle_request(int sck)     override final;

private:
    serverImp _imp;
};*/