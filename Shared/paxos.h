#ifndef PAXOS_H
#define PAXOS_H
#include "istrategy.h"
#include "iconnectionmanager.h"


class Paxos : public IStrategy
{
public:
     explicit Paxos(IConnectionManager& manager):_manager(manager)
     {
     }
     ~Paxos() override = default;

     void SendPrepareID(const uint64_t id) override;
     void SendAcceptRequest(uint64_t id, uint64_t value) override;
private:
     IConnectionManager& _manager;
};



#endif //PAXOS_H
