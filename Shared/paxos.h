#ifndef PAXOS_H
#define PAXOS_H
#include "istrategy.h"
#include "iconnectionmanager.h"
#include "logger.h"

class Paxos : public IStrategy
{
public:
     explicit Paxos(IConnectionManager& manager): _manager(manager), _message_id(0), _prepare_accept_count(0)
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
     uint64_t _message_id;
     uint8_t _prepare_accept_count;
};



#endif //PAXOS_H
