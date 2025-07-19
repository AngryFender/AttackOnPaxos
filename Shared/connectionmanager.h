#ifndef CONNECTIONMANAGER_H
#define CONNECTIONMANAGER_H
#include <shared_mutex>

#include "iacceptoradapter.h"
#include "iconnectionmanager.h"
#include "istrategy.h"
#include "logger.h"

class ConnectionManager: public IConnectionManager
{
public:
    explicit ConnectionManager(IStrategy* strategy, std::shared_ptr<IAcceptorAdapter> adapter): _strategy(strategy), _acceptor(std::move(adapter))
    {
        _strategy->SetConnectionManger(this);

        _acceptor->setHandler([this](const std::shared_ptr<IConnection>& socket)
        {
            this->AcceptConnection(socket);
        });
        _acceptor->open();

    };

    ~ConnectionManager() override
    {
        ConnectionManager::ClearAllConnections();
    };

    void AddConnection(const tcp::endpoint& endpoint, std::shared_ptr<IConnection> socket) override;
    void RemoveConnection(const std::string address) override;
    bool GetConnection(const std::string address, std::shared_ptr<IConnection>& socketAdapter) const override;
    int GetConnectionCount() const override;
    std::map<std::string, std::shared_ptr<IConnection>> GetConnections() const override;
    void AcceptConnection(const std::shared_ptr<IConnection>&) override;
    void ClearAllConnections() override;

    void BroadcastMessage(const std::vector<char>& buffer) override;
    void ReplyMessage(const std::string& address_port, const std::vector<char>& buffer) override;

    ConnectionManager(const ConnectionManager&) = delete;
    ConnectionManager& operator=(const ConnectionManager&) = delete;

    ConnectionManager(ConnectionManager&&) = delete;
    ConnectionManager& operator=(ConnectionManager&&) = delete;

private:
    IStrategy * _strategy;
    std::map<std::string, std::shared_ptr<IConnection>> _out_connections;
    std::shared_ptr<IAcceptorAdapter> _acceptor;
};


#endif //CONNECTIONMANAGER_H
