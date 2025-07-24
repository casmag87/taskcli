#include "TaskClient.h"
#include <iostream>

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "Usage: taskd <command>\n";
        return 1;
    }

    TaskClient client("127.0.0.1", 9001);
    if (!client.connectToServer()) {
        std::cerr << "Failed to connect to server.\n";
        return 1;
    }

    std::string command;
    for (int i = 1; i < argc; ++i) {
        command += std::string(argv[i]) + " ";
    }

    std::string response = client.sendCommand(command);
    std::cout << response << "\n";

    client.disconnect();
    return 0;
}

