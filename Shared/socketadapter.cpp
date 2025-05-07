
#include "socketadapter.h"

tcp::socket& SocketAdapter::getSocket()
{
    return _socket;
}

void SocketAdapter::async_read_some(
    std::function<void(const boost::system::error_code&, std::vector<char>& rawData)> callback)
{
    auto self = get();
    _socket.async_read_some(boost::asio::buffer(_data),[callback, self](const boost::system::error_code& err, std::size_t)
    {
        callback(err, self->_data);
    });
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
