#ifndef ILOGGER_H
#define ILOGGER_H

#include <functional>
#include <string>

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
    virtual ~ILogger();
    virtual ILogger& operator << (const char* message) = 0;
    virtual ILogger& setLogLevel(const LogType& type) = 0;
    virtual bool registerCallback(const LogType type, const std::function<void(const std::string& message)> logCall) = 0;
};

inline ILogger::~ILogger()
{
}

#endif //ILOGGER_H
