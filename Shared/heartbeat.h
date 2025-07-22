#ifndef HEARTBEAT_H
#define HEARTBEAT_H

#include <boost/asio/io_context.hpp>
#include <boost/asio/steady_timer.hpp>
#include "iheartbeat.h"

class Heartbeat: public IHeartbeat
{
public:
    explicit Heartbeat(boost::asio::io_context &io_context, const int milliseconds): _io_context(io_context), _timer(io_context), _milliseconds(milliseconds)
    {
    }

    ~Heartbeat() override = default;
    void Start() override;
    void Stop() override;
    void SetHandler(std::function<void()> handler) override;
private:
    boost::asio::io_context& _io_context;
    boost::asio::steady_timer _timer;
    int _milliseconds;
    std::function<void()> _handler;
};



#endif //HEARTBEAT_H
