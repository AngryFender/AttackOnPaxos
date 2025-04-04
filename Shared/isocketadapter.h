#ifndef ISOCKETADAPTER_H
#define ISOCKETADAPTER_H
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

class ISocketAdapter
{
public:
    virtual ~ISocketAdapter() = default;
    virtual tcp::socket& getSocket() = 0;

    virtual void async_read_some( const boost::asio::mutable_buffer& buffer , std::function<void(const boost::system::error_code&, std::size_t)> callback) = 0;
    virtual void async_write(const boost::asio::const_buffer& message, std::function<void(const boost::system::error_code&, std::size_t)> callback) = 0;
    virtual void connect(const tcp::endpoint& peer_endpoint, boost::system::error_code& ec) = 0;
    virtual void close() = 0;
};

#endif //ISOCKETADAPTER_