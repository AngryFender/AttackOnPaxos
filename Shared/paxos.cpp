#include "paxos.h"
#include "isocketadapter.h"
#include "logger.h"
#include "packet.h"
#include "utility.h"

void Paxos::SetSocketHandlers(const std::shared_ptr<ISocketAdapter>& socket)
{
    socket->set_receive_callback([this](const boost::system::error_code& code, std::vector<char>& data)
    {
        this->ReceivePacket(code, data);
    });
    socket->start_async_receive();

    socket->set_send_callback([](const boost::system::error_code& code)
    {
        if (code)
        {
            Log(ERROR) << "Sending paxos packet error" << code.message().c_str() << "\n";
            return;
        }
        Log(INFO) << "Paxos sent\n";
    });
}

void Paxos::ReceivePacket(const boost::system::error_code& error, std::vector<char>& data)
{
    uint64_t id = 0;
    for(int i = 4; i < 12; ++i)
    {
        id = static_cast<uint64_t>(data[i]<<((i-4)*8)) | id;
    }
    Log(INFO) << "receiving "<<std::to_string(ntohl(id)).c_str() << "\n";
}

void Paxos::SendPrepare(const uint64_t id)
{
    Prepare p{};
    p.id = htonl(id);
    p.type = static_cast<uint32_t>(state::Prepare);
    p.length = htonl(sizeof(p.id) + sizeof (p.type));

    std::vector<char> buffer;
    utility::append_bytes(buffer, p.length);
    utility::append_bytes(buffer, p.id);
    utility::append_bytes(buffer, p.type);

    for(const auto& connection_pair: _manager.GetConnections())
    {
        const auto socket = connection_pair.second;
        socket->async_send((buffer));
    }
}

void Paxos::SendPromise(const uint64_t id, const bool accept, std::shared_ptr<ISocketAdapter> socket)
{
    Promise p{};
    p.id = htonl(id);
    p.type = static_cast<uint8_t>(state::Promise);
    p.accept = static_cast<uint8_t>(accept);
    p.length = htonl(sizeof(p.id) + sizeof (p.type));

    std::vector<char> buffer;
    utility::append_bytes(buffer, p.length);
    utility::append_bytes(buffer, p.id);
    utility::append_bytes(buffer, p.type);
    utility::append_bytes(buffer, p.accept);

    socket->async_send((buffer));
}

void Paxos::SendAccept(const uint64_t id, const uint64_t value)
{
    Accept a{};
    a.id = htonl(id);
    a.type = static_cast<uint8_t>(state::Prepare);
    a.value = htonl(value);
    a.length = htonl(sizeof(a.id) + sizeof(a.type) + sizeof(a.value));

    std::vector<char> buffer;
    utility::append_bytes(buffer, a.length);
    utility::append_bytes(buffer, a.id);
    utility::append_bytes(buffer, a.type);
    utility::append_bytes(buffer, a.value);

    for(const auto& connection_pair: _manager.GetConnections())
    {
        const auto& socket = connection_pair.second;
        socket->async_send(buffer);
    }
}

void Paxos::SendResponse(uint64_t id, const uint64_t value, const bool accept, std::shared_ptr<ISocketAdapter> socket)
{
    Response r{};
    r.id = htonl(id);
    r.type = static_cast<uint8_t>(state::Response);
    r.accept = static_cast<uint8_t>(accept);
    r.value = htonl(value);
    r.length = htonl(sizeof(r.id) + sizeof(r.type) + sizeof(r.accept) + sizeof(value));

    std::vector<char> buffer;
    utility::append_bytes(buffer, r.length);
    utility::append_bytes(buffer, r.id);
    utility::append_bytes(buffer, r.type);
    utility::append_bytes(buffer, r.accept);
    utility::append_bytes(buffer, r.value);

    socket->async_send(buffer);
}
