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
    uint32_t length;
    uint64_t id;
    uint8_t type;
};

struct Promise
{
    uint32_t length;
    uint64_t id;
    uint8_t type;
    uint8_t accept;
};

struct Accept
{
    uint32_t length;
    uint64_t id;
    uint8_t type;
    uint64_t value;
};

struct Response
{
    uint32_t length;
    uint64_t id;
    uint8_t type;
    uint8_t accept;
    uint64_t value;
};


#endif //PACKET_H
