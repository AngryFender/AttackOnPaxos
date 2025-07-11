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
    });
}

void Paxos::SetConnectionManger(IConnectionManager* manager)
{
    if(manager)
        _manager = manager;
}

void Paxos::ReceivePacket(const boost::system::error_code& error, std::vector<char>& data, const std::string& address_port)
{
    const uint8_t node_id = static_cast<uint8_t>(data[4]);
    const uint64_t id = utility::ntohl64(utility::bytes_to_uint<uint64_t>(5, 12, data));
    const state type = static_cast<state>(data[13]);

    switch (type)
    {
    case state::Prepare:
        {
            Log(INFO)<<"Received Prepare packet from Node :"<<std::to_string(node_id).c_str()<<" @ "<<address_port.c_str()<<"\n";
            Log(INFO) << "->Id =" << std::to_string(_local_promise_id).c_str() << "->Value =" << std::to_string(_local_value).c_str() <<"\n\n";
            bool accept = false;
            if(id>_local_promise_id)
            {
                _local_state = state::Accept;
                accept = true;
                _local_promise_id = id;
            }
            SendPromise(_local_promise_id, accept, address_port);
            break;
        }
    case state::Promise:
        {
            Log(INFO)<<"Received Promise packet from Node :"<<std::to_string(node_id).c_str()<<" @ "<<address_port.c_str()<<"\n";
            const bool accepted = static_cast<bool>(data[14]);
            _promise_store.push_back(accepted);

            const int accept_count = std::count(_promise_store.begin(), _promise_store.end(), true);
            const int majority_count = (_manager->GetConnectionCount() + 1)/2 ;

            if (accept_count >= majority_count && _local_state == state::Promise)
            {
                _local_state = state::Accept;
                SendAccept(_local_promise_id, _proposed_value);
            }
            if(accept_count < majority_count && _promise_store.size() == _manager->GetConnectionCount())
            {
                SendPrepare(++_local_promise_id);
            }
            break;
        }
    case state::Accept:
        {
            Log(INFO)<<"Received Accept packet from Node :"<<std::to_string(node_id).c_str()<<" @ "<<address_port.c_str()<<"\n";
            const uint64_t value = utility::ntohl64(utility::bytes_to_uint<uint64_t>(14,21, data));

            bool accept = false;
            if (id >= _local_promise_id && _local_state == state::Accept)
            {
                accept = true;
                _local_value = value;
                _local_promise_id = id;
                _local_state = state::Prepare;
            }

            SendResponse(_local_promise_id, value, accept, address_port);
            Log(INFO) << "->Id =" << std::to_string(_local_promise_id).c_str() << "->Value =" << std::to_string(_local_value).c_str() <<"\n\n";
            break;
        }
    case state::Response:
        {
            Log(INFO)<<"Received Response packet from Node :"<<std::to_string(node_id).c_str()<<" @ "<<address_port.c_str()<<"\n";
            const bool accepted = static_cast<bool>(data[14]);
            const uint64_t value = utility::ntohl64(utility::bytes_to_uint<uint64_t>(15, 22, data));
            _response_store.push_back(accepted);
            _value_store.push_back(value);

            const int accept_count = std::count(_response_store.begin(), _response_store.end(), true);
            const int majority_count = (_manager->GetConnectionCount() + 1)/2 ;

            std::map<uint64_t, int> count_values;
            int max_count = 0;
            for(const auto value: _value_store)
            {
                max_count = std::max(++count_values[value], max_count);
            }

            if (accept_count >= majority_count && max_count >= majority_count && _local_state == state::Accept)
            {
                _local_state = state::Prepare;
                _contribution_handler(contribution_status::success);
            }
            if(accept_count < majority_count && _promise_store.size() == _manager->GetConnectionCount())
            {
                _contribution_handler(contribution_status::consensus_not_met);
            }
        }
        break;
    }
}

void Paxos::ContributeValue(const uint64_t value, std::function<void(const contribution_status&)> handler)
{
    _contribution_handler = handler;
    SendPrepare(++_local_promise_id);
    _proposed_value = value;
}

void Paxos::SendPrepare(const uint64_t id)
{
    Prepare p{};
    p.id = utility::htonl64(id);
    p.type = static_cast<uint8_t>(state::Prepare);
    p.length = htonl(sizeof(p.id) + sizeof(_node_id) + sizeof (p.type));

    std::vector<char> buffer;
    utility::append_bytes(buffer, p.length);
    utility::append_bytes(buffer, _node_id);
    utility::append_bytes(buffer, p.id);
    utility::append_bytes(buffer, p.type);

    _manager->BroadcastMessage(buffer);

    _local_state = state::Promise;
    _promise_store.clear();
    _promise_store.push_back(id > _local_promise_id);
    _local_promise_id = std::max(id, _local_promise_id);
    _response_store.clear();
}

void Paxos::SendPromise(const uint64_t id, const bool accept, const std::string address_port)
{
    Promise p{};
    p.id = utility::htonl64(id);
    p.type = static_cast<uint8_t>(state::Promise);
    p.accept = static_cast<uint8_t>(accept);
    p.length = htonl(sizeof(p.id) + sizeof(_node_id) + sizeof (p.type) + sizeof(p.accept));

    std::vector<char> buffer;
    utility::append_bytes(buffer, p.length);
    utility::append_bytes(buffer, _node_id);
    utility::append_bytes(buffer, p.id);
    utility::append_bytes(buffer, p.type);
    utility::append_bytes(buffer, p.accept);

    _manager->ReplyMessage(address_port, buffer);
}

void Paxos::SendAccept(const uint64_t id, const uint64_t value)
{
    Accept a{};
    a.id = utility::htonl64(id);
    a.type = static_cast<uint8_t>(state::Accept);
    a.value = utility::htonl64(value);
    a.length = htonl(sizeof(a.id) + sizeof(_node_id) + sizeof(a.type) + sizeof(a.value));

    std::vector<char> buffer;
    utility::append_bytes(buffer, a.length);
    utility::append_bytes(buffer, _node_id);
    utility::append_bytes(buffer, a.id);
    utility::append_bytes(buffer, a.type);
    utility::append_bytes(buffer, a.value);

    _manager->BroadcastMessage(buffer);
}

void Paxos::SendResponse(uint64_t id, const uint64_t value, const bool accept, const std::string address_port)
{
    Response r{};
    r.id = utility::htonl64(id);
    r.type = static_cast<uint8_t>(state::Response);
    r.accept = static_cast<uint8_t>(accept);
    r.value = htonl(value);
    r.length = htonl(sizeof(r.id) + sizeof(_node_id) + sizeof(r.type) + sizeof(r.accept) + sizeof(value));

    std::vector<char> buffer;
    utility::append_bytes(buffer, r.length);
    utility::append_bytes(buffer, _node_id);
    utility::append_bytes(buffer, r.id);
    utility::append_bytes(buffer, r.type);
    utility::append_bytes(buffer, r.accept);
    utility::append_bytes(buffer, r.value);

    _manager->ReplyMessage(address_port, buffer);
}
