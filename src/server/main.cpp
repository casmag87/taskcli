#include <iostream>
#include <thread>
#include "Server.h"
#include "ConfigLoader.h"


int main() {
    
    const int DEFAULT_PORT = 9000;
    int port = loadPortFromConfig("config/server.conf", DEFAULT_PORT);

    Server server(port);
    server.start();

    while (true){
	std::this_thread::sleep_for(std::chrono::milliseconds(1));    
    }    

    server.stop();


    return 0;
    
    
}
