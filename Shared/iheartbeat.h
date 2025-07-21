#ifndef IHEARTBEAT_H
#define IHEARTBEAT_H
#include <functional>

class IHeartbeat
{
public:
    virtual ~IHeartbeat() = default;
    virtual void Start() = 0;
    virtual void Stop() = 0;
    virtual void SetHandler(std::function<void()> handler) = 0;
};

#endif //IHEARTBEAT_H
