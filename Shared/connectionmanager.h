#ifndef CONNECTIONMANAGER_H
#define CONNECTIONMANAGER_H
#include "iconnectionmanager.h"

class ConnectionManager: public IConnectionManager
{
public:
    ~ConnectionManager() override = default;
    void AddConnection(std::shared_ptr<ISocketAdapter> socketAdapter) override;
    void RemoveConnection(const std::string address) override;
    std::map<std::string, std::shared_ptr<ISocketAdapter>> GetConnections() const override;

private:
    std::mutex _mutex;
    std::map<std::string, std::shared_ptr<ISocketAdapter>> _connections;
};


#endif //CONNECTIONMANAGER_H
