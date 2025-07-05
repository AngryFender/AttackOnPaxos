#ifndef CONNECTIONMANAGER_H
#define CONNECTIONMANAGER_H
#include <shared_mutex>

#include "iacceptoradapter.h"
#include "iconnectionmanager.h"
#include "logger.h"

class ConnectionManager: public IConnectionManager
{
public:
    explicit ConnectionManager(boost::asio::io_context& io_context, const int port, std::shared_ptr<IAcceptorAdapter> adapter): _io_context(io_context),
        _resolver(io_context), _port(port), _acceptor(std::move(adapter))
    {
        _acceptor->setHandler([this](const std::shared_ptr<ISocketAdapter>& socket)
        {
            this->AcceptConnection(socket);
        });
        _acceptor->open();
    };

    ~ConnectionManager() override
    {
        ConnectionManager::ClearAllConnections();
    };

    void AddConnection(const tcp::endpoint& endpoint, std::shared_ptr<ISocketAdapter> socket) override;
    void RemoveConnection(const std::string address) override;
    bool GetConnection(const std::string address, std::shared_ptr<ISocketAdapter>& socketAdapter) const override;
    int GetConnectionCount() const override;
    std::map<std::string, std::shared_ptr<ISocketAdapter>> GetConnections() const override;
    void AcceptConnection(const std::shared_ptr<ISocketAdapter>&) override;
    void SetSocketHandlers(std::function<void(const std::shared_ptr<ISocketAdapter>& )> callback) override;
    void ClearAllConnections() override;


    ConnectionManager(const ConnectionManager&) = delete;
    ConnectionManager& operator=(const ConnectionManager&) = delete;

    ConnectionManager(ConnectionManager&&) = delete;
    ConnectionManager& operator=(ConnectionManager&&) = delete;

private:
    tcp::resolver _resolver;
    const int _port;
    mutable std::shared_mutex _mutex;
    boost::asio::io_context& _io_context;
    std::map<std::string, std::shared_ptr<ISocketAdapter>> _out_connections;
    std::shared_ptr<IAcceptorAdapter> _acceptor;
    std::function<void(const std::shared_ptr<ISocketAdapter>&)> _set_socket_handlers;
};


#endif //CONNECTIONMANAGER_H
