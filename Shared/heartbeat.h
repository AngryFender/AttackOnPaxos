#ifndef HEARTBEAT_H
#define HEARTBEAT_H

#include <boost/asio/io_context.hpp>
#include <boost/asio/steady_timer.hpp>
#include "iheartbeat.h"

class Heartbeat: public IHeartbeat, std::enable_shared_from_this<Heartbeat>
{
public:
    explicit Heartbeat(boost::asio::io_context& io_context, const int heartbeat_milliseconds, const int ack_milliseconds):
                                                    _heartbeat_timer(io_context),
                                                    _ack_timer(io_context),
                                                    _heartbeat_milliseconds(heartbeat_milliseconds),
                                                    _ack_milliseconds(ack_milliseconds)
    {
    }

    ~Heartbeat() override = default;
    void StartHeartbeat() override;
    void StopHeartbeat() override;
    void SetHeartbeatHandler(std::function<void()> handler) override;
    void StartAckTimeout() override;
    void CancelAckTimeout() override;
    void SetAckTimeoutHandler(std::function<void()> handler) override;
private:
    boost::asio::steady_timer _heartbeat_timer;
    boost::asio::steady_timer _ack_timer;
    int _heartbeat_milliseconds;
    int _ack_milliseconds;
    std::function<void()> _heartbeat_handler;
    std::function<void()> _ack_handler;
};



#endif //HEARTBEAT_H
