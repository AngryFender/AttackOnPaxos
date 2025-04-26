#ifndef CONCORD_H
#define CONCORD_H
#include "iconnectionmanager.h"
#include "istrategy.h"

class Concord {
public:
    explicit Concord(IConnectionManager& connection_manager, IStrategy& strategy):
        _manager(connection_manager), _strategy(strategy)
    {
    }

    IConnectionManager& _manager;
    IStrategy& _strategy;
};

#endif //CONCORD_H
