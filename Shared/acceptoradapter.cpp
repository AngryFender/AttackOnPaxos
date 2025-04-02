#include "acceptoradapter.h"
#include "socketadapter.h"

void AcceptorAdapter::open()
{
    //new socket
    auto socketAdpt  = SocketAdapter::create(_io_context);

    //async_accept
    _acceptor.async_accept(socketAdpt->getSocket(), std::bind(&AcceptorAdapter::handle_accept, this, socketAdpt, boost::asio::placeholders::error));
}

void AcceptorAdapter::handle_accept(const std::shared_ptr<SocketAdapter>& socketAdapter, const error_code& error)
{
    //error handling
    if(!error)
    {
        _connections.push_back(socketAdapter);
    }
    open();
}

const std::vector<std::shared_ptr<ISocketAdapter>>& AcceptorAdapter::getConnections() const
{
    return _connections;
}

