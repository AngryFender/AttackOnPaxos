
#ifndef ISTRATEGY_H
#define ISTRATEGY_H
#include <cstdint>
#include <memory>
#include "isocketadapter.h"

class IStrategy
{
public:
    virtual ~IStrategy() = default;
    virtual void ReceivePacket(const boost::system::error_code& error, std::vector<uint8_t>& data) = 0;
    virtual void SendPrepare(const uint64_t id) = 0;
    virtual void SendPromise(const uint64_t id, const bool accept, std::shared_ptr<ISocketAdapter> socket) = 0;
    virtual void SendAccept(uint64_t id, uint64_t value) = 0;
    virtual void SendResponse(uint64_t id, const uint64_t value, const bool accept, std::shared_ptr<ISocketAdapter> socket) = 0;
    virtual void SetSocketHandlers(const std::shared_ptr<ISocketAdapter>& socket) = 0;
};

#endif //ISTRATEGY_H
