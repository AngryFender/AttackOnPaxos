#ifndef CONNECTIONMANAGER_H
#define CONNECTIONMANAGER_H
#include <shared_mutex>
#include "iconnectionmanager.h"

class ConnectionManager: public IConnectionManager
{
public:
    ~ConnectionManager() override = default;
    explicit ConnectionManager(boost::asio::io_context& io_context,const std::string& port): _resolver(io_context), _port(port)
    {

    };

    void AddConnection(std::shared_ptr<ISocketAdapter> socketAdapter) override;
    void RemoveConnection(const std::string address) override;
    bool GetConnection(const std::string address, std::shared_ptr<ISocketAdapter>& socketAdapter) const override;
    std::map<std::string, std::shared_ptr<ISocketAdapter>> GetConnections() const override;

private:
    tcp::resolver _resolver;
    const std::string _port;
    mutable std::shared_mutex _mutex;
    std::map<std::string, std::shared_ptr<ISocketAdapter>> _connections;
};


#endif //CONNECTIONMANAGER_H
