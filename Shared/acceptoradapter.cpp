#include "acceptoradapter.h"
#include "socketadapter.h"
#include "../Shared/logger.h"

void AcceptorAdapter::setHandler(std::function<void(std::shared_ptr<ISocketAdapter>)> handler)
{
    if (handler)
    {
        _accept_handler = handler;
    }
}

void AcceptorAdapter::open()
{
    //new socket
    const auto socket_adpt  = SocketAdapter::create(_io_context);
    std::shared_ptr<ISocketAdapter> socket_base = socket_adpt;

    //async_accept
    _acceptor.async_accept(socket_adpt->getSocket(), [socket_base, this](const error_code& error)
    {
        if (error)
        {
            Log(ERROR) << "Acceptor Error: " << error.message().c_str() << "\n";
        }
        _accept_handler(socket_base);

        this->open();
    });
}

