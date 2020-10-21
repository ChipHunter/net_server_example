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
    serverImp(int port);
    ~serverImp();

    /* TCP Implementation */
    void sendTcp(int sck, const char* buf);
    void recvTcp(int sck, char* buf, int len);
    int  acceptConnection();

    /* UDP Implementation */
   // void sendUdp(int sck, const char* buf);
   // void recvUdp(int sck, char* buf, int len);

private:
    int _port = -1;
    int _serverSck = -1;
};

class Server {
public:
    virtual ~Server() {};

public:
    virtual void send(int sck, const char* buf)       = 0;
    virtual void recv(int sck, char* buf, int len)    = 0; 
    virtual void handle_request(int sck)              = 0;
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
    int  accept();
    void send(int sck, const char* buf)    override final;
    void recv(int sck, char* buf, int len) override final; 
    void handle_request(int sck)           override final;

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