#include "acceptoradapter.h"

void AcceptorAdapter::init_socket()
{
    //new socket
    //async_accept
}

void AcceptorAdapter::async_accept(std::shared_ptr<ISocketAdapter> socket, std::function<void(error_code& error)>)
{

    _acceptor.async_accept(socket.get(), std::bind(&AcceptorAdapter::handle_accept, this, boost::asio::placeholders::error));
}

void AcceptorAdapter::handle_accept(error_code& error)
{
    //error handling
    init_socket();
}
