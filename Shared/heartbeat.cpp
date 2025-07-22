#include "heartbeat.h"

void Heartbeat::Start()
{
    _timer.expires_after(std::chrono::milliseconds(_milliseconds));
    _timer.async_wait([this](const boost::system::error_code& code)
    {
        this->_handler();
        this->Start();
    });
}

void Heartbeat::Stop()
{
    _timer.cancel();
}

void Heartbeat::SetHandler(std::function<void()> handler)
{
    _handler = handler;
}
