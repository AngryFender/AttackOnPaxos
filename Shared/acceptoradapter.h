#ifndef ACCEPTORADAPTER_H
#define ACCEPTORADAPTER_H
#include "iacceptoradapter.h"

class AcceptorAdapter : public IAcceptorAdapter
{
public:
    AcceptorAdapter(boost::asio::io_context& io_context, const boost::asio::ip::port_type portNo)
        :_io_context(io_context), _acceptor(io_context, tcp::endpoint(tcp::v4(), portNo))
    {
        init_socket();
    }
    ~AcceptorAdapter() override = default;
    void init_socket() override;
    void async_accept(std::shared_ptr<ISocketAdapter> socket, std::function<void(error_code& error)>) override;
    void handle_accept(const error_code& error);

private:
    boost::asio::io_context& _io_context;
    tcp::acceptor _acceptor;
    std::vector<std::shared_ptr<ISocketAdapter>> _connections;
};



#endif //ACCEPTORADAPTER_H
