#ifndef ACCEPTORADAPTER_H
#define ACCEPTORADAPTER_H

#include "iacceptoradapter.h"
#include "socketadapter.h"

class AcceptorAdapter : public IAcceptorAdapter
{
public:
    AcceptorAdapter(boost::asio::io_context& io_context, const boost::asio::ip::port_type port)
        :_io_context(io_context), _acceptor(io_context, tcp::endpoint(tcp::v4(), port))
    {}
    ~AcceptorAdapter() override = default;
    void setHandler(std::function<void(std::shared_ptr<ISocketAdapter>)> handler) override;
    void open() override;

private:
    boost::asio::io_context& _io_context;
    tcp::acceptor _acceptor;
    std::function<void(const std::shared_ptr<ISocketAdapter>&)> _accept_handler;
};



#endif //ACCEPTORADAPTER_H
