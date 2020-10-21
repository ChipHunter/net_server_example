#include "server.h"

serverImp::serverImp(int port) : _port{port} {

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

serverImp::~serverImp() {
    
    if(close(_serverSck) == -1) {
        std::cout << "cant close the socket" << std::endl;
    }

}

void serverImp::sendTcp(int sck, const char* buf) {

    int numOfBytes;
    numOfBytes = write(sck, buf, strlen(buf));

    if(numOfBytes == -1)
        throw std::system_error(errno, std::generic_category());
    else if (numOfBytes != static_cast<int>(strlen(buf)))
        std::cout << "not all bytes were sent with write TCP" << std::endl;

}

void serverImp::recvTcp(int sck, char* buf, int len) {

    int numOfBytes;
    numOfBytes = read(sck, buf, len - 1);

    if(numOfBytes == -1)
        throw std::system_error(errno, std::generic_category());

}

int serverImp::acceptConnection() {

    int cliSck = accept(_serverSck, NULL, NULL);
    if (cliSck == -1)
        throw std::system_error(errno, std::generic_category());

    return cliSck;

}

/**************************************************************************/

TCPServer::TCPServer(int port) : _imp(port) {}

TCPServer::~TCPServer() {}

void TCPServer::send(int sck, const char* buf) {

    _imp.sendTcp(sck, buf);

}

void TCPServer::recv(int sck, char* buf, int len) {

    _imp.recvTcp(sck, buf, len);

}

int TCPServer::accept() {

   return _imp.acceptConnection();

}

void TCPServer::handle_request(int sck) {

    int len = 64;
    char buf[len] = {0};

    recv(sck, buf, len);

    std::cout << "the msg is: " << buf << std::endl;

    send(sck, buf);

    close(sck);

}

/**************************************************************************/