#ifndef SOCKETADAPTER_H
#define SOCKETADAPTER_H
#include "isocketadapter.h"


class SocketAdapter:public ISocketAdapter {

public:
    explicit SocketAdapter(boost::asio::io_context& io_context): _socket(io_context)
    {}
    ~SocketAdapter() override = default;
    tcp::socket& getSocket() override;
    void async_read_some( const boost::asio::mutable_buffer& buffer , std::function<void(const boost::system::error_code&, std::size_t)> callback) override;
    void async_write(const boost::asio::const_buffer& message, std::function<void(const boost::system::error_code&, std::size_t)> callback) override;

private:
    tcp::socket _socket;
};



#endif //SOCKETADAPTER_H
