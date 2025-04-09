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
    virtual void AddConnection(std::shared_ptr<ISocketAdapter> socketAdapter) = 0;
    virtual void RemoveConnection(const std::string address) = 0;
    virtual std::map<std::string, std::shared_ptr<ISocketAdapter>> GetConnections() const = 0;
};

#endif //ICONNECTIONMANAGER_H
