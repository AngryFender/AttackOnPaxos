#ifndef IACCEPTORADAPTER_H
#define IACCEPTORADAPTER_H
#include <functional>
#include <map>
#include <memory>
#include <boost/system/detail/error_code.hpp>
#include "isocketadapter.h"

using boost::system::error_code;
class IAcceptorAdapter
{
public:
    virtual ~IAcceptorAdapter() = default;
    virtual void open() = 0;
    virtual const std::map<std::string,std::shared_ptr<ISocketAdapter>> getConnections() const = 0;
};

#endif //IACCEPTORADAPTER_H
