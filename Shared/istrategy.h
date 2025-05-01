
#ifndef ISTRATEGY_H
#define ISTRATEGY_H
#include <cstdint>

class IStrategy
{
public:
    virtual ~IStrategy() = default;
    virtual void SendPrepareID(const uint64_t id) = 0;
    virtual void SendAcceptRequest(uint64_t id, uint64_t value) = 0;
};

#endif //ISTRATEGY_H
