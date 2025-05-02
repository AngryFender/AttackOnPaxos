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

     void SendPrepare(const uint64_t id) override;
     void SendPromise(const uint64_t id, std::shared_ptr<ISocketAdapter> socket) override;
     void SendAccept(uint64_t id, uint64_t value) override;
     void SendResponse(uint64_t id, std::shared_ptr<ISocketAdapter> socket) override;
private:
     IConnectionManager& _manager;
};



#endif //PAXOS_H
