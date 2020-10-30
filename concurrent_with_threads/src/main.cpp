#include "server.h"

int main() {

    try {

        auto echo = [] (char* buf, int len) {}; 

        std::unique_ptr<Server> serv = std::make_unique<TCPServer>(18000, echo);

        serv->run();

    } catch(const std::system_error& e) {
        
        std::cout << "System Error " << e.what() << std::endl;
    
    } catch (...) {
        
        std::cout << "Another error" << std::endl;
   
    }

    return 0;
}