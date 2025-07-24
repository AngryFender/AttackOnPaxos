#ifndef IHEARTBEAT_H
#define IHEARTBEAT_H
#include <functional>

class IHeartbeat
{
public:
    virtual ~IHeartbeat() = default;
    virtual void StartHeartbeat() = 0;
    virtual void StopHeartbeat() = 0;
    virtual void SetHeartbeatHandler(std::function<void()> handler) = 0;
    virtual void StartAckTimeout() = 0;
    virtual void CancelAckTimeout() = 0;
    virtual void SetAckTimeoutHandler(std::function<void()> handler) = 0;
};

#endif //IHEARTBEAT_H
