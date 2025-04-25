#ifndef CONCORD_H
#define CONCORD_H
#include "iconnectionmanager.h"

class Concord {
public:
    explicit Concord(IConnectionManager& connection_manager): _manager(connection_manager)
    {

    }
    IConnectionManager& _manager;
};

#endif //CONCORD_H
