#ifndef LOGGER_H
#define LOGGER_H
#include <string>
#include "ilogger.h"


class Logger: ILogger
{
private:
     LogCallBack _debugCall;
     LogCallBack _infoCall;
     LogCallBack _warningCall;
     LogCallBack _errorCall;
     LogCallBack _fatalCall;

public:
     Logger(std::string file);
     ~Logger() override;
     ILogger& operator <<(const char* message) override;
     ILogger& setLogLevel(const LogType& type) override;
     void registerDebugCallback(LogType& type, LogCallBack logCall) override;
};



#endif //LOGGER_H
