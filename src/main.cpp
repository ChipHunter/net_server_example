#include "server.h"

int main() {

    int sck;

    try {

        Server* serv{new TCPServer(18000)};

        sck = dynamic_cast<TCPServer*>(serv)->accept();

        serv->handle_request(sck);
        
        delete serv;

    } catch(const std::system_error& e) {
        
        std::cout << "System Error " << e.what() << std::endl;
    
    } catch (...) {
        
        std::cout << "Another error" << std::endl;
   
    }

    return 0;
}