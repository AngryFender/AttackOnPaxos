#include "connectionmanager.h"

void ConnectionManager::AddConnection(std::shared_ptr<ISocketAdapter> socketAdapter)
{
}

void ConnectionManager::RemoveConnection(const std::string address)
{

}

std::map<std::string, std::shared_ptr<ISocketAdapter>> ConnectionManager::GetConnections() const
{
    return _connections;
}