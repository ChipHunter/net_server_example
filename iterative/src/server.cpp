#include "server.h"

void serverImp::setupTcp(int port) {

    _port = port;

    sockaddr_in serverAddr;
    
    _serverSck = socket(AF_INET, SOCK_STREAM, 0);
    if(_serverSck == -1)
        throw std::system_error(errno, std::generic_category());
    
    serverAddr.sin_port = htons(_port);
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);

    if(bind(_serverSck, (sockaddr*)&serverAddr, sizeof(serverAddr)) == -1)
        throw std::system_error(errno, std::generic_category());

    if(listen(_serverSck, 10) == -1)
        throw std::system_error(errno, std::generic_category());

}

void serverImp::closeTcp() {
    
    if(close(_serverSck) == -1) {
        std::cout << "cant close the socket " << strerror(errno) << std::endl;
    }

    if (_clientSck != -1) { 
        if(close(_clientSck) == -1) {
            std::cout << "cant close the socket " << strerror(errno) << std::endl;
        }
    }

}

void serverImp::sendTcp(const char* buf) {

    int numOfBytes;
    numOfBytes = write(_clientSck, buf, strlen(buf));

    if(numOfBytes == -1)
        throw std::system_error(errno, std::generic_category());
    else if (numOfBytes != static_cast<int>(strlen(buf)))
        std::cout << "not all bytes were sent with write TCP" << std::endl;

}

void serverImp::recvTcp(char* buf, int len) {

    int numOfBytes;
    numOfBytes = read(_clientSck, buf, len - 1);

    if(numOfBytes == -1)
        throw std::system_error(errno, std::generic_category());

}

void serverImp::acceptConnection() {

    if (_clientSck != -1) {
        if(close(_clientSck) == -1) {
            std::cout << "cant close the socket " << strerror(errno) << std::endl;
        }
    }

    _clientSck = accept(_serverSck, NULL, NULL);
    if (_clientSck == -1)
        throw std::system_error(errno, std::generic_category());

}


void serverImp::setupUdp(int port) {

    _port = port;

    sockaddr_in serverAddr;

    _serverSck = socket(AF_INET, SOCK_DGRAM, 0);
    if(_serverSck == -1)
        throw std::system_error(errno, std::generic_category());

    serverAddr.sin_port = htons(_port);
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);

    if(bind(_serverSck, (sockaddr*)&serverAddr, sizeof(serverAddr)) == -1)
        throw std::system_error(errno, std::generic_category());

}

void serverImp::closeUdp() {

    if (close(_serverSck) == -1)
        throw std::system_error(errno, std::generic_category());

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

/**************************************************************************/

TCPServer::TCPServer(int port, std::function<void(char*, int)> func) : _func{func} {

    _imp.setupTcp(port);

}

TCPServer::~TCPServer() {

    _imp.closeTcp();

}

void TCPServer::send(const char* buf) {

    _imp.sendTcp(buf);

}

void TCPServer::recv(char* buf, int len) {

    _imp.recvTcp(buf, len);

}

void TCPServer::accept() {
    
    _imp.acceptConnection();

}

void TCPServer::run() {

    int len = 64;
    char buf[len] = {0};

    accept();

    recv(buf, len);

    _func(buf, len);

    send(buf);

}

/**************************************************************************/

UDPServer::UDPServer(int port, std::function<void(char*, int)> func) : _func{func} {
    
    _imp.setupUdp(port);

}

UDPServer::~UDPServer() {

    _imp.closeUdp();

}

void UDPServer::send(const char* buf) {

    _imp.sendUdp(buf);

}

void UDPServer::recv(char* buf, int len) {

    _imp.recvUdp(buf, len);

}

void UDPServer::run() {

    int len = 64;
    char buf[len] = {0};

    recv(buf, len);

    _func(buf, len);

    send(buf);

}