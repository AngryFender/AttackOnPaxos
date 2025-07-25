#include "heartbeat.h"

void Heartbeat::StartHeartbeat()
{
    _heartbeat_timer.expires_after(std::chrono::milliseconds(_heartbeat_milliseconds));
    auto self = shared_from_this();
    _heartbeat_timer.async_wait([self](const boost::system::error_code& code)
    {
        if(self->_heartbeat_handler)
        {
            self->_heartbeat_handler();
        }
        self->StartHeartbeat();
    });
}

void Heartbeat::StopHeartbeat()
{
    _heartbeat_timer.cancel();
}

void Heartbeat::SetHeartbeatHandler(std::function<void()> handler)
{
    _heartbeat_handler = handler;
}

void Heartbeat::StartAckTimeout()
{
    _ack_timer.expires_after(std::chrono::milliseconds(_ack_milliseconds));
    auto self = shared_from_this();
    _ack_timer.async_wait([self](const boost::system::error_code& code)
    {
        if(self->_ack_handler)
        {
            self->_ack_handler();
        }
    });}

void Heartbeat::CancelAckTimeout()
{
    _ack_timer.cancel();
}

void Heartbeat::SetAckTimeoutHandler(std::function<void()> handler)
{
    _ack_handler = handler;
}
