#include "ConfigLoader.h"
#include <fstream>
#include <iostream>
#include <sstream>

// Loads a port number from a configuration file
// If the file doesn't exist or the port is invalid, returns the defaultPort
int loadPortFromConfig(const std::string& filename, int defaultPort) {
    std::ifstream file(filename);

    // If the config file can't be opened, fall back to default
    if (!file.is_open()) {
        std::cout << "Config file not found, using default port " << defaultPort << "\n";
        return defaultPort;
    }

    std::string line;
    while (std::getline(file, line)) {
        // Skip empty lines or comment lines (starting with #)
        if (line.empty() || line[0] == '#') continue;

        std::istringstream iss(line);
        std::string key;

        // Split line into key and value at the '=' character
        if (std::getline(iss, key, '=')) {
            std::string valueStr;
            if (std::getline(iss, valueStr)) {
                // Look for the "port" key
                if (key == "port") {
                    try {
                        int port = std::stoi(valueStr);

                        // Validate port range
                        if (port > 0 && port < 65536) {
                            std::cout << "Using port from config: " << port << "\n";
                            return port;
                        } else {
                            std::cerr << "Port number out of valid range, using default port " << defaultPort << "\n";
                        }
                    } catch (...) {
                        // Handle non-integer values gracefully
                        std::cerr << "Invalid port number in config, using default port " << defaultPort << "\n";
                    }
                }
            }
        }
    }

    // If no valid "port" key is found, use default
    std::cout << "Port not set in config, using default port " << defaultPort << "\n";
    return defaultPort;
}


