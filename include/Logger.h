#pragma once
#include <string_view>
#include <source_location>
#include <string>
// Correct usage of macro parameter X
#define TCI_LOG(X) Logger::Log(X);
#define TCI_ERROR(X) Logger::Error(X);
class Logger
{
public:
        Logger() = default;
        ~Logger() = default;

        static void Log(const std::string_view message);
        static void Error(const std::string_view message, std::source_location location = std::source_location::current());

private:
        static std::string CurrentDate();
};


