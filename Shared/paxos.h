#ifndef PAXOS_H
#define PAXOS_H
#include "istrategy.h"
#include "iconnectionmanager.h"

class Paxos : public IStrategy
{
public:
     explicit Paxos(IConnectionManager& manager):_manager(manager)
     {
         for (const auto& connection_pair : _manager.GetConnections())
         {
             const auto& socket = connection_pair.second;
             socket->async_read_some([&](const boost::system::error_code& code, std::vector<char>& data)
             {
                 this->ReceivePacket(code, data);
             });
         }
     }

     ~Paxos() override = default;

     void ReceivePacket(const boost::system::error_code& error, std::vector<char>& data) override;
     void SendPrepare(const uint64_t id) override;
     void SendPromise(const uint64_t id, const bool accept, std::shared_ptr<ISocketAdapter> socket) override;
     void SendAccept(uint64_t id, uint64_t value) override;
     void SendResponse(uint64_t id, const uint64_t value, const bool accept, std::shared_ptr<ISocketAdapter> socket) override;
private:
     IConnectionManager& _manager;
};



#endif //PAXOS_H
