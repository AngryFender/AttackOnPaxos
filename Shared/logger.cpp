#include "logger.h"

Logger::~Logger()
{
}

ILogger& Logger::getInstance()
{
    std::call_once(_initFlag, []()
    {
       _logger = std::unique_ptr<ILogger>(new Logger());
    });
    return *_logger;
}

void Logger::setInstance(std::unique_ptr<ILogger>&& logger)
{
    if(_logger == nullptr)
    {
        _logger = std::move(logger);
    }
}

ILogger& Logger::operator<<(const char* message)
{
    std::lock_guard<std::mutex> lockGuard(_mutex);
    switch(_logType)
    {
    case DEBUG: _debugCall(message);break;
    case INFO: _infoCall(message); break;
    case WARNING: _warningCall(message); break;
    case ERROR: _errorCall(message); break;
    case FATAL: _fatalCall(message); break;
    }

    return *_logger;
} 

ILogger& Logger::setLogLevel(const LogType& type)
{
    std::lock_guard<std::mutex> lockGuard(_mutex);
    _logType = type;
    return *_logger;
}

bool Logger::registerCallback(LogType& type, std::function<void(const std::string& message)> logCall)
{
    std::lock_guard<std::mutex> lockGuard(_mutex);
    switch (type)
    {
        case DEBUG: _debugCall = logCall; break;
        case INFO: _infoCall = logCall; break;
        case WARNING: _warningCall = logCall; break;
        case ERROR:_errorCall = logCall; break;
        case FATAL:_fatalCall = logCall; break;
        default: break;
    }
}
