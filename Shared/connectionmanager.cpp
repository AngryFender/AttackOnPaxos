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
        this->_out_connections[socket->getSocket().remote_endpoint().address().to_string()] = socket;
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

std::map<std::string, std::shared_ptr<ISocketAdapter>> ConnectionManager::GetConnections() const
{
    std::shared_lock lock(_mutex);
    return _out_connections;
}

void ConnectionManager::AcceptConnection(const std::shared_ptr<ISocketAdapter>& socket)
{
    const std::string address = socket->getSocket().remote_endpoint().address().to_string() ;
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
