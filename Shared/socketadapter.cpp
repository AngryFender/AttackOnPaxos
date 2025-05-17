#include "socketadapter.h"
#include "logger.h"
#include "utility.h"
#include "boost/asio/write.hpp"
tcp::socket& SocketAdapter::getSocket()
{
    return _socket;
}

void SocketAdapter::set_receive_callback(std::function<void(const boost::system::error_code&, std::vector<uint8_t>& rawData)> callback)
{
    _receive_callback = std::move(callback);
}

void SocketAdapter::set_send_callback(std::function<void(const boost::system::error_code&)> callback)
{
}

std::shared_ptr<SocketAdapter> SocketAdapter::create(boost::asio::io_context& io_context)
{
    return std::make_shared<SocketAdapter>(io_context);
}

std::shared_ptr<SocketAdapter> SocketAdapter::get()
{
    return shared_from_this();
}

void SocketAdapter::async_connect(const tcp::endpoint& peer_endpoint,const std::function<void(const boost::system::error_code&)> callback)
{
    _socket.async_connect( peer_endpoint, callback);
}

void SocketAdapter::close()
{
    _socket.close();
}

void SocketAdapter::start_async_receive()
{
    auto self = get();
    _socket.async_read_some(boost::asio::buffer(_temp_data),[self](const boost::system::error_code& err, std::size_t)
    {
        if (err) {
            Log(ERROR) << "Reading from socket failed:" << err.message().c_str() << "\n";
            return;
        }

        //implement framing layer
        //add to persistent buffer
        for(const uint8_t& item: self->_temp_data)
        {
            self->_internal_buff.push_back(item);
        }

        //try to parse the message
        if (utility::parse_message(self->_internal_buff, self->_packet_data))
        {
            //callback the message handler
            self->_receive_callback(err, self->_packet_data);
        }

        //rehook
        self->start_async_receive();
    });
}

void SocketAdapter::async_send(const boost::asio::const_buffer& message)
{
    _outbounds.emplace(message);

    if (!_write_in_progress)
    {
        _write_in_progress = true;
        start_async_send();
    }
}

void SocketAdapter::start_async_send()
{
    auto self = get();
    async_write(_socket,_outbounds.front(),[self](const boost::system::error_code& error, size_t size)
    {
        self->_outbounds.pop();
        if(self->_outbounds.empty() || error)
        {
            self->_write_in_progress = false;
            self->_send_callback(error);
            return;
        }
        self->start_async_send();
    });
}


