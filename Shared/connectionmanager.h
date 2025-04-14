#ifndef CONNECTIONMANAGER_H
#define CONNECTIONMANAGER_H
#include <shared_mutex>
#include "iconnectionmanager.h"

class ConnectionManager: public IConnectionManager, public std::enable_shared_from_this<ConnectionManager>
{
public:
    explicit ConnectionManager(boost::asio::io_context& io_context,const int port): _io_context(io_context), _resolver(io_context), _port(port)
    {
    };

    void AddConnection(const std::string& address) override;
    void RemoveConnection(const std::string address) override;
    bool GetConnection(const std::string address, std::shared_ptr<ISocketAdapter>& socketAdapter) const override;
    std::map<std::string, std::shared_ptr<ISocketAdapter>> GetConnections() const override;

    ~ConnectionManager() override = default;

    ConnectionManager(const ConnectionManager&) = delete;
    ConnectionManager& operator=(const ConnectionManager&) = delete;

    ConnectionManager(ConnectionManager&&) = delete;
    ConnectionManager& operator=(ConnectionManager&&) = delete;

private:
    tcp::resolver _resolver;
    const int _port;
    mutable std::shared_mutex _mutex;
    boost::asio::io_context& _io_context;
    std::map<std::string, std::shared_ptr<ISocketAdapter>> _connections;
};


#endif //CONNECTIONMANAGER_H
