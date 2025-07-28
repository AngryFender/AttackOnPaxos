#ifndef ACCEPTORADAPTER_H
#define ACCEPTORADAPTER_H

#include "iacceptoradapter.h"
#include "connection.h"

class AcceptorAdapter : public IAcceptorAdapter
{
public:
    AcceptorAdapter(boost::asio::io_context& io_context, const boost::asio::ip::port_type port, const int heartbeat_timeout, const int ack_timeout)
        :_io_context(io_context), _acceptor(io_context, tcp::endpoint(tcp::v4(), port)), _heartbeat_timeout(heartbeat_timeout), _ack_timeout(ack_timeout)
    {}
    ~AcceptorAdapter() override = default;
    void setHandler(std::function<void(std::shared_ptr<IConnection>)> handler) override;
    void open() override;

private:
    boost::asio::io_context& _io_context;
    tcp::acceptor _acceptor;
    int _heartbeat_timeout;
    int _ack_timeout;
    std::function<void(const std::shared_ptr<IConnection>&)> _accept_handler;
};



#endif //ACCEPTORADAPTER_H
