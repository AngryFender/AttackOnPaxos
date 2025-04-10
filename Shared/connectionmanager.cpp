#include "connectionmanager.h"

void ConnectionManager::AddConnection(std::shared_ptr<ISocketAdapter> socketAdapter)
{
    std::lock_guard lock(_mutex);
    _connections[socketAdapter->getSocket().remote_endpoint().address().to_string()] = socketAdapter;
}

void ConnectionManager::RemoveConnection(const std::string address)
{
    std::lock_guard lock(_mutex);
    _connections.erase(address);
}

bool ConnectionManager::GetConnection(const std::string address, std::shared_ptr<ISocketAdapter>& socketAdapter) const
{
    std::lock_guard lock(_mutex);
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
    std::lock_guard lock(_mutex);
    return _connections;
}
