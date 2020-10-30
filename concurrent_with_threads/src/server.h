#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include <memory>
#include <functional>
#include <thread>
#include <vector>
#include <queue>
#include <mutex>

class jobsQueue {
public:
    void enqueue(int sck);
    int  dequeue();

private:
    std::queue<int> _jobsQueue;
    std::mutex _mtx;
};

class serverWorkers {
public:
    serverWorkers(int numOfThreads, std::shared_ptr<jobsQueue> queue, std::function<void(char*, int)> func);
    ~serverWorkers();

private:
    void sendTcp(int sck, const char* buf);
    void recvTcp(int sck, char* buf, int len);
    void funcToRunTcp();
    void joinThreads();

private:
    std::shared_ptr<jobsQueue> _jobQueue;
    std::function<void(char*, int)> _func;
    std::vector<std::thread*> _threadVect;
};


class serverMain {
public:
    serverMain(std::shared_ptr<jobsQueue> queue);
    void setupTcp(int port);
    void closeTcp();
    void runTcp();
    void acceptConnection();
    //void setupUdp(int port, std::function<void(char*, int)> func);
    //void closeUdp();
    //void runUdp();
    
private:
    //void sendUdp(const char* buf);
    //void recvUdp(char* buf, int len);
    //void funcToRunTcp();
    //void funcToRunUdp();

private:
    int _port = -1;
    int _serverSck = -1;
    std::shared_ptr<jobsQueue> _jobQueue;
};

class serverImp {
public:
    serverImp(std::function<void(char*, int)> func);
    void setupTcp(int port);
    void closeTcp();
    void runTcp();

private:
    std::shared_ptr<jobsQueue>   _jobs;
    std::unique_ptr<serverMain>    _main;
    std::unique_ptr<serverWorkers> _workers;
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

/*class UDPServer : public Server {
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
    serverMain _imp;
};*/