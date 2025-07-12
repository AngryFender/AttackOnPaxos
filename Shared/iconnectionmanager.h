#ifndef ICONNECTIONMANAGER_H
#define ICONNECTIONMANAGER_H
#include <map>
#include <memory>
#include "iconnectionmanager.h"
#include "isocketadapter.h"

class IConnectionManager
{
public:
    virtual ~IConnectionManager() = default;
    virtual void AddConnection(const tcp::endpoint& endpoint, std::shared_ptr<ISocketAdapter> socket) = 0;
    virtual void RemoveConnection(const std::string address) = 0;
    virtual bool GetConnection(const std::string address, std::shared_ptr<ISocketAdapter>& socketAdapter) const = 0;
    virtual int GetConnectionCount() const = 0;
    virtual void AcceptConnection(const std::shared_ptr<ISocketAdapter>& socket) = 0;
    virtual std::map<std::string, std::shared_ptr<ISocketAdapter>> GetConnections() const = 0;
    virtual void ClearAllConnections() = 0;

    virtual void BroadcastMessage(const std::vector<char>& buffer) = 0;
    virtual void ReplyMessage(const std::string& address_port, const std::vector<char>& buffer) = 0;
};
#endif //ICONNECTIONMANAGER_H
