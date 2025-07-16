
#ifndef ISTRATEGY_H
#define ISTRATEGY_H
#include <cstdint>
#include <memory>

#include "iconnectionmanager.h"

enum class contribution_status
{
    success,
    consensus_not_met,
};

class IStrategy
{
public:
    virtual ~IStrategy() = default;
    virtual void SetConnectionManger(IConnectionManager* manager) = 0;
    virtual void ReceivePacket(std::vector<char>& data, const std::string& address_port) = 0;
    virtual void ContributeValue(const uint64_t value, std::function<void(const contribution_status &)> handler) = 0;
};

#endif //ISTRATEGY_H
