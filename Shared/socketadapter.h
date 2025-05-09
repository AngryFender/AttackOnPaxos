#ifndef SOCKETADAPTER_H
#define SOCKETADAPTER_H
#include "isocketadapter.h"


class SocketAdapter:public ISocketAdapter, std::enable_shared_from_this<SocketAdapter> {

public:
    explicit SocketAdapter(boost::asio::io_context& io_context): _socket(io_context), _data(1024)
    {}
    ~SocketAdapter() override = default;
    tcp::socket& getSocket() override;

    void async_read_some(std::function<void(const boost::system::error_code&, std::vector<char>& rawData)> callback) override;
    void async_write(const boost::asio::const_buffer& message, std::function<void(const boost::system::error_code&, std::size_t)> callback) override;
    static std::shared_ptr<SocketAdapter> create(boost::asio::io_context& io_context);
    std::shared_ptr<SocketAdapter> get();
    void async_connect(const tcp::endpoint& peer_endpoint, const std::function<void(const boost::system::error_code&)> callback) override;
    void close() override;
private:
    tcp::socket _socket;
    std::vector<char> _data;
    std::vector<char> _temp_data;
    void parse_message();
};



#endif //SOCKETADAPTER_H
