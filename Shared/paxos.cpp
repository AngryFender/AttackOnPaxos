#include "paxos.h"
#include "isocketadapter.h"
#include "logger.h"
#include "packet.h"

void Paxos::SendPrepare(const uint64_t id)
{
    //send prepare id to all acceptor nodes
    Prepare p{};
    p.id = htonl(id);
    p.type = static_cast<int>(state::Prepare);
    p.length = htonl(sizeof(p.id) + sizeof (p.type));

    for(const auto& connection_pair: _manager.GetConnections())
    {
        const auto& socket = connection_pair.second;
        socket->async_write(boost::asio::const_buffer(&p,sizeof(p)), [](const boost::system::error_code& code, std::size_t size)
        {
            Log(INFO)<<"Write Complete " << std::to_string(size).c_str() <<"\n";
        });
    }
}

void Paxos::SendPromise(const uint64_t id, std::shared_ptr<ISocketAdapter> socket)
{
}

void Paxos::SendAccept(const uint64_t id, const uint64_t value)
{
    //send accept request id to all acceptor nodes
    Accept a{};
    a.id = htonl(id);
    a.type = static_cast<int>(state::Prepare);
    a.value = htonl(value);
    a.length = htonl(sizeof(a.id) + sizeof (a.type));

    for(const auto& connection_pair: _manager.GetConnections())
    {
        const auto& socket = connection_pair.second;
        socket->async_write(boost::asio::const_buffer(&a,sizeof(a)), [](const boost::system::error_code& code, std::size_t size)
        {
            Log(INFO)<<"Write Complete " << std::to_string(size).c_str() <<"\n";
        });
    }
}

void Paxos::SendResponse(uint64_t id, std::shared_ptr<ISocketAdapter> socket)
{
}
