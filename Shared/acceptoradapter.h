#ifndef ACCEPTORADAPTER_H
#define ACCEPTORADAPTER_H
#include "iacceptoradapter.h"

class AcceptorAdapter : public IAcceptorAdapter
{
public:
    AcceptorAdapter(boost::asio::io_context& io_context, const boost::asio::ip::port_type portNo)
        :_acceptor(io_context, tcp::endpoint(tcp::v4(), portNo))
    {
        init_socket();
    }
    ~AcceptorAdapter() override = default;
    void init_socket() override;
    void async_accept(std::shared_ptr<ISocketAdapter> socket, std::function<void(error_code& error)>) override;
    void handle_accept(error_code& error);

private:
    tcp::acceptor _acceptor;

};



#endif //ACCEPTORADAPTER_H
