#ifndef HEARTBEAT_H
#define HEARTBEAT_H

#include "iheartbeat.h"

class Heartbeat: public IHeartbeat
{
    std::function<void()> _handler;
public:
    ~Heartbeat() = default;
    void Start() override;
    void Stop() override;
    void SetHandler(std::function<void()> handler) override;
};



#endif //HEARTBEAT_H
