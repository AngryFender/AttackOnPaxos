#ifndef ILOGGER_H
#define ILOGGER_H

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
    virtual void registerDebugCallback(LogType& type, LogCallBack logCall) = 0;
};

#endif //ILOGGER_H
