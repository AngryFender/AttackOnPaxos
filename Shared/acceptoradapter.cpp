#include "acceptoradapter.h"
#include "connection.h"
#include "../Shared/logger.h"

void AcceptorAdapter::setHandler(std::function<void(std::shared_ptr<IConnection>)> handler)
{
    _accept_handler = handler;
}

void AcceptorAdapter::open()
{
    //new socket
    std::shared_ptr<IConnection> socket_base = std::make_shared<Connection>(_io_context);

    //async_accept
    _acceptor.async_accept(socket_base->getSocket(), [socket_base, this](const error_code& error)
    {
        if (error)
        {
            Log(ERROR) << "Acceptor Error: " << error.message().c_str() << "\n";
        }
        _accept_handler(socket_base);

        this->open();
    });
}

