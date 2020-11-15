#include "server.h"

using Byte = std::vector<std::byte>;

void jobsQueue::enqueue(int sck) {

    std::unique_lock<std::mutex> lock(_mtx);
    _jobsQueue.push(sck);
    _cv.notify_all();

}

int jobsQueue::dequeue() {

    std::unique_lock<std::mutex> lock(_mtx);
    while(_jobsQueue.empty())
        _cv.wait(lock);

    int sck = _jobsQueue.front();
    _jobsQueue.pop();

    return sck;

}

serverMain::serverMain(std::shared_ptr<jobsQueue> queue) : _jobQueue{queue} {}

void serverMain::setupTcp(int port) {

    _port = port;

    sockaddr_in serverAddr;
    
    _serverSck = socket(AF_INET, SOCK_STREAM, 0);
    if(_serverSck == -1)
        throw std::system_error(errno, std::generic_category());
    
    try {
        serverAddr.sin_port = htons(_port);
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);

        if(bind(_serverSck, (sockaddr*)&serverAddr, sizeof(serverAddr)) == -1)
            throw std::system_error(errno, std::generic_category());

        if(listen(_serverSck, 10) == -1)
            throw std::system_error(errno, std::generic_category());

    } catch(const std::system_error& e) {

        if(close(_serverSck) == -1) 
            std::cout << "can't close the socket" << std::endl;

        throw e;

    }

}

void serverMain::acceptConnection() {

    int clientSck = accept(_serverSck, NULL, NULL);
    if (clientSck == -1)
        throw std::system_error(errno, std::generic_category());
    else {
        _jobQueue->enqueue(clientSck);
    }

}

void serverMain::closeTcp() {
    
    if(close(_serverSck) == -1) {
        std::cout << "cant close the socket " << strerror(errno) << std::endl;
    }

}

void serverMain::runTcp() {

    while(true) {

        acceptConnection();

    }
}

serverWorkers::serverWorkers(int numOfThreads, std::shared_ptr<jobsQueue> queue, 
                            std::function<void(Byte&, int)> func) : _jobQueue{queue}, _func{func} {

    try {

        for (int i = 0; i < numOfThreads; ++i) {

            std::thread* t = new std::thread(&serverWorkers::funcToRunTcp, this);
            _threadVect.push_back(t);

        }

    } catch (const std::system_error& e) {

        joinThreads();

        throw e;

    } catch (const std::bad_alloc& ba) {

        joinThreads();

        throw ba;

    }

}

serverWorkers::~serverWorkers() {

    joinThreads();

}

void serverWorkers::sendTcp(int sck, const char* buf) {

    int numOfBytes;
    
    numOfBytes = write(sck, buf, strlen(buf));
    if(numOfBytes == -1)
        std::cout << "Error in write: " << strerror(errno) << std::endl;
    else if (numOfBytes != static_cast<int>(strlen(buf)))
        std::cout << "not all bytes were sent with write TCP" << std::endl;

}

void serverWorkers::recvTcp(int sck, char* buf, int len) {

    int numOfBytes;

    numOfBytes = read(sck, buf, len - 1);
    if(numOfBytes == -1)
        std::cout << "Error in read: " << strerror(errno) << std::endl;

}

void serverWorkers::joinThreads() {

    for(auto i : _threadVect) {
        i->join();
        delete i;
    }

}

void serverWorkers::funcToRunTcp() {

    int sck;

    while(true) {
    
        sck = _jobQueue->dequeue();

        int len = 64;
        Byte buf;
        buf.reserve(len);

        recvTcp(sck, reinterpret_cast<char*>(&buf[0]), len);

        _func(buf, len);

        sendTcp(sck, reinterpret_cast<char*>(&buf[0]));

        close(sck);

    }

}

serverImp::serverImp(std::function<void(Byte&, int)> func) : _jobs{std::make_shared<jobsQueue>()}, 
                           _main{std::make_unique<serverMain>(_jobs)}, 
                           _workers{std::make_unique<serverWorkers>(10, _jobs, func)} {}

void serverImp::setupTcp(int port)  {

    _main->setupTcp(port);

}

void serverImp::runTcp() {

    _main->runTcp();

}

void serverImp::closeTcp() {

    _main->closeTcp();

}

/*void serverImp::setupUdp(int port, std::function<void(char*, int)> func) {

    _port = port;
    _func = func;

    sockaddr_in serverAddr;

    _serverSck = socket(AF_INET, SOCK_DGRAM, 0);
    if(_serverSck == -1)
        throw std::system_error(errno, std::generic_category());

    serverAddr.sin_port = htons(_port);
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);

    if(bind(_serverSck, (sockaddr*)&serverAddr, sizeof(serverAddr)) == -1)
        throw std::system_error(errno, std::generic_category());

//    createThreadsUdp();

}

void serverImp::closeUdp() {

    if (close(_serverSck) == -1)
        throw std::system_error(errno, std::generic_category());

//    joinThreads();

}

void serverImp::sendUdp(const char* buf) {

    int numOfBytes;

    numOfBytes = sendto(_serverSck, buf, strlen(buf), 0, (sockaddr*)&_srcAddr, sizeof(_srcAddr));

    if(numOfBytes == -1)
        throw std::system_error(errno, std::generic_category());
    else if (numOfBytes != static_cast<int>(strlen(buf)))
        std::cout << "not all bytes were sent with sendto UDP" << std::endl;
    
}

void serverImp::recvUdp(char* buf, int len) {

    int numOfBytes;
    socklen_t addrlen = sizeof(_srcAddr);
    
    numOfBytes = recvfrom(_serverSck, buf, len - 1, 0, (sockaddr*)&_srcAddr, &addrlen);

    if(numOfBytes == -1)
        throw std::system_error(errno, std::generic_category());

}

void serverImp::runUdp() {

    int len = 64;
    char buf[len] = {0};

    while(true) {
    
        recvUdp(buf, len);

        _func(buf, len);

        sendUdp(buf);
    
    }

}
*/
/*void serverImp::funcToRunUdp() {

    std::unique_lock<std::mutex> lock(_mtx);

    if(!_jobs.empty()) {
       	int sck = _jobs.front();
        _jobs.pop();

        lock.unlock();

        int len = 64;
        char buf[len];

        recvTcp(sck, buf, len);

        _func(buf, len);

        sendTcp(sck, buf);

    } else {

        sleep(0.1);

    }

}

void serverImp::createThreadsUdp() {

    for (int i = 0; i < _numOfThreads; ++i) {

        std::thread* t = new std::thread(&serverImp::funcToRunUdp, this);
        _threadVect.push_back(t);

    }

}
*/




/**************************************************************************/

TCPServer::TCPServer(int port, std::function<void(Byte&, int)> func) : _imp(func) {

    _imp.setupTcp(port);

}

TCPServer::~TCPServer() {

    _imp.closeTcp();

}

void TCPServer::run() {

    _imp.runTcp();

}

/**************************************************************************/

/*UDPServer::UDPServer(int port, std::function<void(char*, int)> func) {
    
    _imp.setupUdp(port, func);

}

UDPServer::~UDPServer() {

    _imp.closeUdp();

}

void UDPServer::run() {

    _imp.runUdp();

}*/