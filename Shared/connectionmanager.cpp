#include "connectionmanager.h"
#include "logger.h"
#include "socketadapter.h"

void ConnectionManager::AddConnection(const std::string& address)
{
    std::unique_lock lock(_mutex);

    const tcp::endpoint endpoint(boost::asio::ip::address::from_string(address), _port);
    std::shared_ptr<ISocketAdapter> socket = std::make_shared<SocketAdapter>(_io_context);
    socket->async_connect(endpoint,[socket, address,this](const boost::system::error_code& code)
    {
        if(code)
        {
            Log(ERROR)<<"Unable to connect to "<< address.c_str() << code.to_string().c_str() << "\n";
            return;
        }
        std::unique_lock lock(this->_mutex);
        this->_out_connections[socket->getSocket().remote_endpoint().address().to_string()] = socket;
        Log(INFO)<<"Connected to " << address.c_str()<<"\n";
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
