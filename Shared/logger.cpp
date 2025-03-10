#include "logger.h"

Logger::~Logger()
{
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
}

void Logger::registerDebugCallback(LogType& type, LogCallBack logCall)
{
    std::lock_guard<std::mutex> lockGuard(_mutex);
    switch (type)
    {
    case DEBUG: _debugCall = logCall; break;
    case INFO: _infoCall = logCall; break;
    case WARNING: _warningCall = logCall; break;
    case ERROR:_errorCall = logCall; break;
    case FATAL:_fatalCall = logCall; break;
    default:
    }
}
