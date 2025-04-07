#include "acceptoradapter.h"
#include "socketadapter.h"
#include "../Shared/logger.h"

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
        //add thread synchronisation

        std::string address = socketAdapter->getSocket().remote_endpoint().address().to_string();
        _connections[address] =  socketAdapter;
        Log(DEBUG) << address.c_str() <<" has successfully connected\n";
    }
    open();
}

const std::map<std::string, std::shared_ptr<ISocketAdapter>> AcceptorAdapter::getConnections() const
{
    //add thread synchronisation

    return _connections;
}
