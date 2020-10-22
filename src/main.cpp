#include "server.h"

int main() {

    int sck;

    try {

        std::unique_ptr<Server> serv{new TCPServer(18000)};

        dynamic_cast<TCPServer*>(serv.get())->accept();

        serv->handle_request();

    } catch(const std::system_error& e) {
        
        std::cout << "System Error " << e.what() << std::endl;
    
    } catch (...) {
        
        std::cout << "Another error" << std::endl;
   
    }

    return 0;
}