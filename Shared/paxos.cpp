#include "paxos.h"

void Paxos::SendPrepareID(const uint64_t id)
{
    //send prepare id to all acceptor nodes
    Proposal p;
    p.id = htonl(id);
    p.type = State::Prepare;
    p.length = htonl(sizeof(p.id) + sizeof (p.type));

    for(const auto& connection_pair: _manager->GetConnections())
    {
        const auto& socket = connection_pair.second;
        
    }
    
}

void Paxos::SendAcceptRequest(const uint64_t id)
{
    //send accept request id to all acceptor nodes
}
