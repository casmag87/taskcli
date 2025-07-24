#include "Logger.h"
#include <chrono>
#include <ctime>
#include <iostream>
constexpr const char* GREEN = "\033[0;32m";
constexpr const char* RED = "\033[0;31m";
constexpr const char* WHITE = "\033[0m";  // Reset to default
std::string Logger::CurrentDate()
{
    auto now = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(now);

    std::string output(30, '\0');
    std::strftime(&output[0], output.size(), "%y-%m-%d %H:%M:%S", std::localtime(&now_c));
    
    return output;
}

void Logger::Log(const std::string_view message)
{
    std::cout << GREEN << "LOG: " << CurrentDate() << " - " << message << WHITE << "\n";
}

// Definition without the default argument
void Logger::Error(const std::string_view message, std::source_location location)
{
    std::cout << RED << "ERROR: " << CurrentDate() << " - " << message 
              << "\nFILE: " << location.file_name()
              << "\nFUNC: " << location.function_name()
              << "\nLINE: " << location.line() << WHITE << "\n\n";
}
