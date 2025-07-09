#ifndef PAXOS_H
#define PAXOS_H
#include "istrategy.h"
#include "iconnectionmanager.h"
#include "logger.h"
#include "packet.h"

class Paxos : public IStrategy
{
public:
     explicit Paxos(IConnectionManager& manager, const uint8_t node_id): _manager(manager),
                                                  _local_state(state::Prepare),
                                                  _node_id(node_id),
                                                  _local_promise_id(0),
                                                  _promise_store(3, false),
                                                  _response_store(3, false),
                                                  _local_value(0),
                                                  _proposed_value(0)
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
     void ReceivePacket(const boost::system::error_code& error, std::vector<char>& data, const std::string& address_port) override;
     void ContributeValue(const uint64_t value, std::function<void(const contribution_status &)> handler) override;
     void SetSocketHandlers(const std::shared_ptr<ISocketAdapter>& socket) override;
private:
     IConnectionManager& _manager;
     uint8_t _node_id;
     state _local_state;
     uint64_t _local_promise_id;
     std::vector<bool> _promise_store;
     std::vector<bool> _response_store;
     std::vector<uint64_t> _value_store;
     uint64_t _local_value;
     uint64_t _proposed_value;
     std::function<void(const contribution_status&)> _contribution_handler;

     void SendPrepare(const uint64_t id);
     void SendPromise(const uint64_t id, const bool accept, const std::string address_port);
     void SendAccept(uint64_t id, uint64_t value);
     void SendResponse(uint64_t id, const uint64_t value, const bool accept, const std::string address_port);
};



#endif //PAXOS_H
