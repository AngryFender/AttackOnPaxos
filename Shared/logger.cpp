#include "logger.h"

Logger::~Logger()
{
}

ILogger& Logger::operator<<(const char* message)
{

}

ILogger& Logger::setLogLevel(const LogType& type)
{
}

void Logger::registerDebugCallback(LogType& type, LogCallBack logCall)
{
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
