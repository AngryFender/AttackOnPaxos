#ifndef ILOGGER_H
#define ILOGGER_H

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
    virtual void registerDebugCallback(LogDebug logDebug) = 0;
    virtual void registerInfoCallback(LogInfo logInfo) = 0;
    virtual void registerWarningCallback(LogWarning logWarning) = 0;
    virtual void registerErrorCallback(LogError logError) = 0;
    virtual void registerFatalCallback(LogFatal logFatal) = 0;
};

#endif //ILOGGER_H
