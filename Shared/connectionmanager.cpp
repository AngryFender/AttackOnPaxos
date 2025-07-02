#include "connectionmanager.h"
#include "logger.h"

void ConnectionManager::AddConnection(const std::string& address, const tcp::endpoint& endpoint, std::shared_ptr<ISocketAdapter>& socket)
{
    socket->async_connect(endpoint,[socket, address,this](const error_code& code)
    {
        if(code)
        {
            Log(ERROR)<<"Unable to connect to "<< address.c_str() << code.to_string().c_str() << "\n";
            return;
        }
        std::unique_lock lock(this->_mutex);
        const std::string full_address = socket->getSocket().remote_endpoint().address().to_string() + ":" + std::to_string(socket->getSocket().remote_endpoint().port());
        this->_out_connections[full_address] = socket;
        Log(INFO)<<"Connected to " << address.c_str()<<"\n";

        if (_set_socket_handlers)
        {
            _set_socket_handlers(socket);
        }
    });
}

void ConnectionManager::RemoveConnection(const std::string address)
{
    std::unique_lock lock(_mutex);
    _out_connections.erase(address);
}

bool ConnectionManager::GetConnection(const std::string address, std::shared_ptr<ISocketAdapter>& socketAdapter) const
{
    std::shared_lock lock(_mutex);
    const auto it = _out_connections.find(address);
    if(it != _out_connections.end())
    {
        socketAdapter = it->second;
        return true;
    }
    return false;
}

int ConnectionManager::GetConnectionCount() const
{
   return _out_connections.size();
}

std::map<std::string, std::shared_ptr<ISocketAdapter>> ConnectionManager::GetConnections() const
{
    std::shared_lock lock(_mutex);
    return _out_connections;
}

void ConnectionManager::AcceptConnection(const std::shared_ptr<ISocketAdapter>& socket)
{
    const std::string address = socket->getSocket().remote_endpoint().address().to_string() + ":" + std::to_string(socket->getSocket().remote_endpoint().port());
    if (!_out_connections.contains(address))
    {
        _out_connections[address] = socket;
        Log(INFO)<<"Accepting connection from "<<address.c_str()<<"\n";

        if(_set_socket_handlers)
        {
            _set_socket_handlers(socket);
        }
    } 
}

void ConnectionManager::SetSocketHandlers(std::function<void(const std::shared_ptr<ISocketAdapter>&)> callback)
{
    _set_socket_handlers = callback;
}

void ConnectionManager::ClearAllConnections()
{
    for (auto connection_pair : _out_connections)
    {
        auto socket = connection_pair.second;
        socket->set_receive_callback(nullptr);
        socket->set_send_callback(nullptr);
        socket->close();
    }
}
