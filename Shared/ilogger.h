#ifndef ILOGGER_H
#define ILOGGER_H

#include <functional>
#include <string>
typedef void (*LogCallBack)(const char* message);

enum LogType
{
    DEBUG = 0,
    INFO,
    WARNING,
    ERROR,
    FATAL
};

class ILogger
{
public:
    virtual ~ILogger() = default;
    virtual ILogger& operator << (const char* message) = 0;
    virtual ILogger& setLogLevel(const LogType& type) = 0;
    virtual bool registerCallback(LogType& type, std::function<void(const std::string& message)> LogCallBack);
};

#endif //ILOGGER_H
