#include "socketadapter.h"
#include "logger.h"

tcp::socket& SocketAdapter::getSocket()
{
    return _socket;
}

void SocketAdapter::set_receive_callback(std::function<void(const boost::system::error_code&, std::vector<uint8_t>& rawData)> callback)
{
    _receive_callback = std::move(callback);
}

void SocketAdapter::async_write(const boost::asio::const_buffer& message,
    std::function<void(const boost::system::error_code&, std::size_t)> callback)
{
    _socket.async_write_some(message, callback);
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
        if (self->parse_message(self->_internal_buff))
        {
            //callback the message handler
            self->_receive_callback(err, self->_packet_data);
        }

        //rehook
        self->start_async_receive();
    });
}
bool SocketAdapter::parse_message(boost::circular_buffer<char>& buffer)
{
    return false;
}