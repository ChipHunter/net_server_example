#include "server.h"


int main() {

    try {

        auto echo = [] (char* buf, int len) {

            snprintf(buf, len, "Thank You!");
        }; 

        std::unique_ptr<Server> serv = std::make_unique<UDPServer>(18000, echo);

        serv->run();

    } catch(const std::system_error& e) {
        
        std::cout << "System Error " << e.what() << std::endl;
    
    } catch (...) {
        
        std::cout << "Another error" << std::endl;
   
    }

    return 0;
}