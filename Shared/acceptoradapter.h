#ifndef ACCEPTORADAPTER_H
#define ACCEPTORADAPTER_H

#include "iacceptoradapter.h"
#include "socketadapter.h"

class AcceptorAdapter : public IAcceptorAdapter
{
public:
    AcceptorAdapter(boost::asio::io_context& io_context, const boost::asio::ip::port_type portNo)
        :_io_context(io_context), _acceptor(io_context, tcp::endpoint(tcp::v4(), portNo))
    {
        open();
    }
    ~AcceptorAdapter() override = default;
    void open() override;
    void handle_accept(const std::shared_ptr<SocketAdapter>& socketAdapter, const error_code& error);
    const std::map<std::string,std::shared_ptr<ISocketAdapter>> getConnections() const override;

private:
    boost::asio::io_context& _io_context;
    tcp::acceptor _acceptor;
    std::map<std::string, std::shared_ptr<ISocketAdapter>> _connections;
};



#endif //ACCEPTORADAPTER_H
