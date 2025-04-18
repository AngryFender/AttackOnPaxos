#ifndef IACCEPTORADAPTER_H
#define IACCEPTORADAPTER_H
#include <functional>
#include <map>
#include <memory>
#include "isocketadapter.h"

using boost::system::error_code;
class IAcceptorAdapter
{
public:
    virtual ~IAcceptorAdapter() = default;
    virtual void setHandler(std::function<void(std::shared_ptr<ISocketAdapter>)> handler) = 0;
    virtual void open() = 0;
};

#endif //IACCEPTORADAPTER_H
