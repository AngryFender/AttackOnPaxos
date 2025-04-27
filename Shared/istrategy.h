
#ifndef ISTRATEGY_H
#define ISTRATEGY_H

class IStrategy
{
public:
    virtual ~IStrategy() = default;
    virtual void SendPrepareID(const uint64_t id) = 0;
    virtual void SendAcceptRequest(const uint64_t id) = 0;
};

#endif //ISTRATEGY_H
