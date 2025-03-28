#ifndef LOGGER_H
#define LOGGER_H
#include <memory>
#include <mutex>
#include <functional>
#include "ilogger.h"

class Logger: public ILogger
{
private:
     static inline std::unique_ptr<ILogger> _logger = nullptr;
     static inline std::once_flag _initFlag;
     std::mutex _mutex;
     LogType _logType;

     typedef std::function<void(const std::string& message)> LogCallBack;
     LogCallBack _debugCall;
     LogCallBack _infoCall;
     LogCallBack _warningCall;
     LogCallBack _errorCall;
     LogCallBack _fatalCall;

public:
     ~Logger() override;
     static ILogger& getInstance();
     static void setInstance(std::unique_ptr<ILogger>&& logger);
     ILogger& operator <<(const char* message) override;
     ILogger& setLogLevel(const LogType& type) override;
     bool registerCallback(LogType type, std::function<void(const std::string& message)> logCall) override;
private:
     Logger() = default;
};

#define Log(level) Logger::getInstance().setLogLevel(LogType::level)



#endif //LOGGER_H
