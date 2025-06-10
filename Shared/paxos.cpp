#include "paxos.h"
#include "isocketadapter.h"
#include "logger.h"
#include "packet.h"
#include "utility.h"

void Paxos::SetSocketHandlers(const std::shared_ptr<ISocketAdapter>& socket)
{
    socket->set_receive_callback([this](const boost::system::error_code& code, std::vector<char>& data, std::shared_ptr<ISocketAdapter> socket)
    {
        this->ReceivePacket(code, data, socket);
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

void Paxos::ReceivePacket(const boost::system::error_code& error, std::vector<char>& data, std::shared_ptr<ISocketAdapter>& socket)
{
    const uint64_t id = utility::ntohl64(utility::bytes_to_uint<uint64_t>(4, 11, data));
    const auto type = static_cast<state>(data[12]);

    switch (type)
    {
    case state::Prepare:
        {
            bool accept = false;
            if(id>_promise_id)
            {
                accept = true;
                _promise_id = id;
            }
            SendPromise(_promise_id, accept, socket);
            break;
        }
    case state::Promise:
        {
            const bool accepted = static_cast<bool>(data[13]);
            accepted && ++_prepare_accept_count;

            const int majority_count = (_manager.GetConnectionCount() + 1) / 2;
            if(_prepare_accept_count >= majority_count)
            {
                SendAccept(_promise_id, _value);
            }
            break;
        }
    case state::Accept:
        {
            const uint64_t value = utility::ntohl64(utility::bytes_to_uint<uint64_t>(13,20, data));

            bool accept = false;
            if(id>_promise_id)
            {
                accept = true;
                _value = value;
                _promise_id = id;
            }

            SendResponse(_promise_id, value, accept, socket);
            break;
        }
    case state::Response: break;
    }

    Log(INFO) << "packet size:"<<std::to_string(ntohl(utility::bytes_to_uint<uint32_t>(0,3,data))).c_str() << " ";
    Log(INFO) << "receiving id:" << std::to_string(id).c_str() << " ";
    Log(INFO) << "receiving type:"<<std::to_string(static_cast<uint8_t>(type)).c_str() << "\n";
}

void Paxos::SendPrepare(const uint64_t id)
{
    Prepare p{};
    p.id = utility::htonl64(id);
    p.type = static_cast<uint8_t>(state::Prepare);
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

    _prepare_accept_count = 0;
}

void Paxos::SendPromise(const uint64_t id, const bool accept, std::shared_ptr<ISocketAdapter> socket)
{
    Promise p{};
    p.id = utility::htonl64(id);
    p.type = static_cast<uint8_t>(state::Promise);
    p.accept = static_cast<uint8_t>(accept);
    p.length = htonl(sizeof(p.id) + sizeof (p.type)+ sizeof(p.accept));

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
    a.id = utility::htonl64(id);
    a.type = static_cast<uint8_t>(state::Accept);
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
    r.id = utility::htonl64(id);
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
