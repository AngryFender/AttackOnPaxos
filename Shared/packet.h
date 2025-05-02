#ifndef PACKET_H
#define PACKET_H
#include <cstdint>

enum class state:int8_t
{
    Prepare = 0,
    Promise,
    Accept,
    Response
};

struct Prepare
{
    uint64_t id;
    uint8_t type;
    uint32_t length;
};

struct Promise
{
    uint64_t id;
    uint32_t length;
    uint8_t type;
    uint8_t accept;
};

struct Accept
{
    uint64_t id;
    uint32_t length;
    uint8_t type;
    uint64_t value;
};

struct Response
{
    uint64_t id;
    uint32_t length;
    uint8_t type;
    uint8_t accept;
    uint64_t value;
};


#endif //PACKET_H
