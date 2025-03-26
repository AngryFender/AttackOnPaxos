
#include "socketadapter.h"

tcp::socket& SocketAdapter::getSocket()
{
    return _socket;
}

void SocketAdapter::async_read_some(const boost::asio::mutable_buffer& buffer,
                                    std::function<void(const boost::system::error_code&, std::size_t)> callback)
{
    _socket.async_read_some(buffer, callback);
}

void SocketAdapter::async_write(const boost::asio::const_buffer& message,
    std::function<void(const boost::system::error_code&, std::size_t)> callback)
{
    _socket.async_write_some(message, callback);
}
