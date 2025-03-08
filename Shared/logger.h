#ifndef LOGGER_H
#define LOGGER_H
#include <string>
#include "ilogger.h"


class Logger: ILogger
{
public:
     Logger(std::string file);
     ~Logger() override;
     ILogger& operator << (const char* message) override;
     ILogger& setLogLevel(const LogType& type) override;
     void registerDebugCallback(LogDebug logDebug) override;
     void registerInfoCallback(LogInfo logInfo) override;
     void registerWarningCallback(LogWarning logWarning) override;
     void registerErrorCallback(LogError logError) override;
     void registerFatalCallback(LogFatal logFatal) override;
};



#endif //LOGGER_H
