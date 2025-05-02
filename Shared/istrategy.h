
#ifndef ISTRATEGY_H
#define ISTRATEGY_H
#include <cstdint>
#include <memory>
#include "isocketadapter.h"

class IStrategy
{
public:
    virtual ~IStrategy() = default;
    virtual void SendPrepare(const uint64_t id) = 0;
    virtual void SendPromise(const uint64_t id, std::shared_ptr<ISocketAdapter> socket) = 0;
    virtual void SendAccept(uint64_t id, uint64_t value) = 0;
    virtual void SendResponse(uint64_t id, std::shared_ptr<ISocketAdapter> socket) = 0;
};

#endif //ISTRATEGY_H
