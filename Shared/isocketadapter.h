#ifndef ISOCKETADAPTER_H
#define ISOCKETADAPTER_H
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

class ISocketAdapter
{
public:
    virtual ~ISocketAdapter() = default;
    virtual tcp::socket& getSocket() = 0;

    virtual void set_receive_callback(std::function<void(const boost::system::error_code&, std::vector<char>&, std::shared_ptr<ISocketAdapter>)> callback) = 0;
    virtual void start_async_receive() = 0;
    virtual void set_send_callback(std::function<void(const boost::system::error_code&)> callback) = 0;
    virtual void async_send(std::vector<char>& message) = 0;
    virtual void async_connect(const tcp::endpoint& peer_endpoint, std::function<void(const boost::system::error_code&)> callback) = 0;
    virtual void close() = 0;
};

#endif //ISOCKETADAPTER_