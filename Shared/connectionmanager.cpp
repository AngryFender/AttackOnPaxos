#include "connectionmanager.h"
#include "socketadapter.h"

void ConnectionManager::AddConnection(const std::string& address)
{
    std::unique_lock lock(_mutex);
    const tcp::resolver::query query(address,_port,boost::asio::ip::resolver_base::numeric_service);
    tcp::resolver::results_type endpoints = _resolver.resolve(query);

    std::shared_ptr<ISocketAdapter> socket = std::make_shared<SocketAdapter>(_io_context);
    // socket->async_connect(endpoints,[self = shared_from_this()](const boost::system::error_code& code, const tcp::endpoint& endpoint)
    // {
    // });
}

void ConnectionManager::RemoveConnection(const std::string address)
{
    std::unique_lock lock(_mutex);
    _connections.erase(address);
}

bool ConnectionManager::GetConnection(const std::string address, std::shared_ptr<ISocketAdapter>& socketAdapter) const
{
    std::shared_lock lock(_mutex);
    const auto it = _connections.find(address);
    if(it != _connections.end())
    {
        socketAdapter = it->second;
        return true;
    }
    return false;
}

std::map<std::string, std::shared_ptr<ISocketAdapter>> ConnectionManager::GetConnections() const
{
    std::shared_lock lock(_mutex);
    return _connections;
}
