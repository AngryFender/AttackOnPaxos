#include "connectionmanager.h"

void ConnectionManager::AddConnection(std::shared_ptr<ISocketAdapter> socketAdapter)
{
    std::unique_lock lock(_mutex);
    _connections[socketAdapter->getSocket().remote_endpoint().address().to_string()] = socketAdapter;

    // tcp::resolver::query query(ipAddress,_port,boost::asio::ip::resolver_base::numeric_service);
    // tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
    // tcp::resolver::iterator end;
    //
    // tcp::socket socket(io_context);
    // boost::system::error_code error = boost::asio::error::host_not_found;
    // while (error && endpoint_iterator != end)
    // {
    //     socket.close();
    //     socket.connect(*endpoint_iterator++, error);
    // }
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
