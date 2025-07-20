#ifndef IHEARTBEAT_H
#define IHEARTBEAT_H
#include <functional>

class IHeartbeat
{
public:
    virtual ~IHeartbeat() = default;
    virtual Start() = 0;
    virtual Stop() = 0;
    virtual SetHandler(std::function<void()> handler) = 0;
};

#endif //IHEARTBEAT_H
