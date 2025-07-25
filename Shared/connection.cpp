#include "connection.h"
#include "logger.h"
#include "utility.h"
#include "boost/asio/write.hpp"
tcp::socket& Connection::getSocket()
{
    return _socket;
}

void Connection::set_receive_callback(std::function<void(std::vector<char>& rawData)> callback)
{
    _receive_callback = callback;
}

void Connection::set_send_callback(std::function<void(const boost::system::error_code&)> callback)
{
    _send_callback = callback;
}

void Connection::async_connect(const tcp::endpoint& peer_endpoint,const std::function<void(const boost::system::error_code&)> callback)
{
    _socket.async_connect( peer_endpoint, callback);
}

void Connection::close()
{
    _socket.close();
}

void Connection::start_async_receive()
{
    auto self = shared_from_this();
    _socket.async_read_some(boost::asio::buffer(_temp_data),[self](const boost::system::error_code& err, std::size_t size)
    {
        if (err) {
            Log(ERROR) << "Reading from socket failed:" << err.message().c_str() << "\n";
            return;
        }
        self->_temp_data.resize(size);

        //implement framing layer
        //add to persistent buffer
        for(const char& item: self->_temp_data)
        {
            self->_internal_buff.push_back(item);
        }

        //try to parse the message
        if (utility::parse_message(self->_internal_buff, self->_packet_data))
        {
            //callback the message handler
            self->_receive_callback(self->_packet_data);
            self->_packet_data.clear();
        }

        //rehook
        self->start_async_receive();
    });
}

void Connection::async_send(const std::vector<char>& message)
{
    _outbounds.push(message);

    if (!_write_in_progress)
    {
        _write_in_progress = true;
        start_async_send();
    }
}

void Connection::start_async_send()
{
    auto self = shared_from_this();
    const auto buffer = boost::asio::buffer(_outbounds.front());
    async_write(_socket, buffer, [self](const boost::system::error_code& error, size_t size)
    {
        self->_outbounds.pop();
        if (self->_outbounds.empty() || error)
        {
            self->_write_in_progress = false;
            self->_send_callback(error);
            return;
        }
        self->start_async_send();
    });
}