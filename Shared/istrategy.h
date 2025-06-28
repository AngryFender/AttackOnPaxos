
#ifndef ISTRATEGY_H
#define ISTRATEGY_H
#include <cstdint>
#include <memory>
#include "isocketadapter.h"

enum class contribution_status
{
    success,
    consensus_not_met,
};

class IStrategy
{
public:
    virtual ~IStrategy() = default;
    virtual void ReceivePacket(const boost::system::error_code& error, std::vector<char>& data, std::shared_ptr<ISocketAdapter>& socket) = 0;
    virtual void ContributeValue(const uint64_t value, std::function<void(const contribution_status &)> handler) = 0;
    virtual void SetSocketHandlers(const std::shared_ptr<ISocketAdapter>& socket) = 0;
};

#endif //ISTRATEGY_H
