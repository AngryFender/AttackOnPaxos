#ifndef PAXOS_H
#define PAXOS_H
#include "istrategy.h"
#include "iconnectionmanager.h"
#include "logger.h"
#include "packet.h"

class Paxos : public IStrategy
{
public:
     explicit Paxos(IConnectionManager& manager): _manager(manager),
                                                  _local_state(state::Prepare),
                                                  _local_promise_id(0),
                                                  _promise_store(3, false),
                                                  _local_value(0)
     {
          _manager.SetSocketHandlers([this](const std::shared_ptr<ISocketAdapter>& socket)
          {
               this->SetSocketHandlers(socket);
          });
     }

     ~Paxos() override
     {
          _manager.ClearAllConnections();
     };
     void ReceivePacket(const boost::system::error_code& error, std::vector<char>& data, std::shared_ptr<ISocketAdapter>& socket) override;
     void SendPrepare(const uint64_t id) override;
     void SendPromise(const uint64_t id, const bool accept, std::shared_ptr<ISocketAdapter> socket) override;
     void SendAccept(uint64_t id, uint64_t value) override;
     void SendResponse(uint64_t id, const uint64_t value, const bool accept, std::shared_ptr<ISocketAdapter> socket) override;
     void SetSocketHandlers(const std::shared_ptr<ISocketAdapter>& socket) override;
private:
     IConnectionManager& _manager;
     state _local_state;
     uint64_t _local_promise_id;
     std::vector<bool> _promise_store;
     uint64_t _local_value;
};



#endif //PAXOS_H
