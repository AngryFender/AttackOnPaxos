#ifndef SOCKETADAPTER_H
#define SOCKETADAPTER_H
#include <queue>
#include <boost/circular_buffer.hpp>
#include "isocketadapter.h"

class SocketAdapter:public ISocketAdapter, public std::enable_shared_from_this<SocketAdapter> {

public:
    explicit SocketAdapter(boost::asio::io_context& io_context): _socket(io_context), _write_in_progress(false), _internal_buff(1024), _temp_data(1024)
    {
        _write_in_progress = false;
    }
    ~SocketAdapter() override = default;
    tcp::socket& getSocket() override;
    void set_receive_callback(std::function<void(const boost::system::error_code&, std::vector<char>& rawData, std::shared_ptr<ISocketAdapter> socket)> callback) override;
    void set_send_callback(std::function<void(const boost::system::error_code&)> callback) override;
    void start_async_receive() override;
    void async_send(const std::vector<char>& message) override;
    void async_connect(const tcp::endpoint& peer_endpoint, const std::function<void(const boost::system::error_code&)> callback) override;
    void close() override;
private:
    tcp::socket _socket;
    bool _write_in_progress;
    boost::circular_buffer<char> _internal_buff;
    std::vector<char> _packet_data;
    std::vector<char> _temp_data;
    std::queue<std::vector<char>> _outbounds;
    std::function<void(const boost::system::error_code&)> _send_callback;
    std::function<void(const boost::system::error_code&, std::vector<char>& rawData, std::shared_ptr<ISocketAdapter>)> _receive_callback;
    void start_async_send();
};



#endif //SOCKETADAPTER_H
