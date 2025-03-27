#ifndef IACCEPTORADAPTER_H
#define IACCEPTORADAPTER_H
#include <functional>
#include <memory>
#include <boost/system/detail/error_code.hpp>
#include "isocketadapter.h"

using boost::system::error_code;
class IAcceptorAdapter
{
public:
    virtual ~IAcceptorAdapter() = default;
    virtual void init_socket() = 0;
    virtual void async_accept(std::shared_ptr<ISocketAdapter> socket, std::function<void(error_code& error)>) = 0;
};

#endif //IACCEPTORADAPTER_H
