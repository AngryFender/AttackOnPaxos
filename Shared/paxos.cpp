#include "paxos.h"
#include "isocketadapter.h"
#include "logger.h"
#include "packet.h"

void Paxos::SendPrepare(const uint64_t id)
{
    //send prepare id to all acceptor nodes
    Prepare p{};
    p.id = htonl(id);
    p.type = static_cast<uint8_t>(state::Prepare);
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

void Paxos::SendPromise(const uint64_t id, const bool accept, std::shared_ptr<ISocketAdapter> socket)
{
    Promise p{};
    p.id = htonl(id);
    p.type = static_cast<uint8_t>(state::Promise);
    p.accept = static_cast<uint8_t>(accept);
    p.length = htonl(sizeof(p.id) + sizeof (p.type));
    socket->async_write(boost::asio::const_buffer(&p, sizeof(p)),[](const boost::system::error_code& code, std::size_t size)
    {
        Log(INFO) << "Write Complete " << std::to_string(size).c_str() << "\n";
    });
}

void Paxos::SendAccept(const uint64_t id, const uint64_t value)
{
    //send accept request id to all acceptor nodes
    Accept a{};
    a.id = htonl(id);
    a.type = static_cast<uint8_t>(state::Prepare);
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
