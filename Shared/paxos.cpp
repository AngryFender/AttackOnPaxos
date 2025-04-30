#include "paxos.h"
#include "isocketadapter.h"
#include "logger.h"
#include "packet.h"

void Paxos::SendPrepareID(const uint64_t id)
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

void Paxos::SendAcceptRequest(const uint64_t id)
{
    //send accept request id to all acceptor nodes
}
