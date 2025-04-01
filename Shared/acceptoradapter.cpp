#include "acceptoradapter.h"
#include "socketadapter.h"

void AcceptorAdapter::open()
{
    //new socket
    auto socketAdpt  = SocketAdapter::create(_io_context);
    _connections.emplace_back(socketAdpt);

    //async_accept
    _acceptor.async_accept(socketAdpt->getSocket(), std::bind(&AcceptorAdapter::handle_accept, this,  boost::asio::placeholders::error));
}

void AcceptorAdapter::handle_accept( const error_code& error)
{
    //error handling
    if(error)
    open();
}
