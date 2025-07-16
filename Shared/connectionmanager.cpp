#include "connectionmanager.h"
#include "logger.h"

void ConnectionManager::AddConnection(const tcp::endpoint& endpoint, std::shared_ptr<ISocketAdapter> socket)
{
    const std::string address_port = endpoint.address().to_string() + ":" + std::to_string(endpoint.port());

    socket->async_connect(endpoint, [address_port,socket,this](const error_code& code)
    {
        if(code)
        {
            Log(ERROR) << "Unable to connect to " << address_port.c_str() << " -> " << code.to_string().c_str() << "\n";
            return;
        }
        this->_out_connections[address_port] = socket;
        Log(INFO)<<"Connected from "<<socket->getSocket().local_endpoint().address().to_string().c_str() << ":" << std::to_string(socket->getSocket().local_endpoint().port()).c_str()<<" to " << address_port.c_str()<<"\n";

        socket->set_receive_callback([this, address_port](std::vector<char>& buffer)
        {
            this->_strategy->ReceivePacket(buffer, address_port);
        });
        socket->set_send_callback([this, address_port](const error_code& code)
        {
            if(code)
                Log(INFO) << "Unable to send packets to " << address_port.c_str() << "(, error code:)" << code.to_string().c_str() << "\n";
        });
        socket->start_async_receive();
    });
}

void ConnectionManager::RemoveConnection(const std::string address)
{
    _out_connections.erase(address);
}

bool ConnectionManager::GetConnection(const std::string address, std::shared_ptr<ISocketAdapter>& socketAdapter) const
{
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
    return _out_connections;
}

void ConnectionManager::AcceptConnection(const std::shared_ptr<ISocketAdapter>& socket)
{
    const std::string address_port = socket->getSocket().remote_endpoint().address().to_string() + ":" + std::to_string(socket->getSocket().remote_endpoint().port());
    if (!_out_connections.contains(address_port))
    {
        _out_connections[address_port] = socket;
        Log(INFO)<<"Accepting connection from "<<address_port.c_str()<<"\n";

        socket->set_receive_callback([this, address_port](std::vector<char>& buffer)
        {
            this->_strategy->ReceivePacket(buffer, address_port);
        });
        socket->set_send_callback([this, address_port](const error_code& code)
        {
            if(code)
                Log(INFO) << "Unable to send packets to " << address_port.c_str() << "(, error code:)" << code.to_string().c_str() << "\n";
        });
        socket->start_async_receive();
    }
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

void ConnectionManager::BroadcastMessage(const std::vector<char>& buffer)
{
    for(const auto& pair: _out_connections)
    {
        pair.second->async_send(buffer);
    }
}

void ConnectionManager::ReplyMessage(const std::string& address_port, const std::vector<char>& buffer)
{
    if(_out_connections.contains(address_port))
    {
        _out_connections[address_port]->async_send(buffer);
    }
}

