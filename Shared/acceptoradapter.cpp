#include "acceptoradapter.h"
#include "socketadapter.h"

void AcceptorAdapter::init_socket()
{
    //new socket
    auto socketAdpt = std::make_shared<SocketAdapter>(_io_context);
    _connections.emplace_back(socketAdpt);

    //async_accept
    _acceptor.async_accept(socketAdpt->getSocket(), std::bind(&AcceptorAdapter::handle_accept, this,  boost::asio::placeholders::error));
}

void AcceptorAdapter::async_accept(std::shared_ptr<ISocketAdapter> socket, std::function<void(error_code& error)>)
{

}

void AcceptorAdapter::handle_accept( const error_code& error)
{
    //error handling
    if(error)
    init_socket();
}
