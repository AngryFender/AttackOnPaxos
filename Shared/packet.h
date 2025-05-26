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
    uint32_t length;
    uint8_t type;
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
    uint64_t value;
    uint32_t length;
    uint8_t type;
};

struct Response
{
    uint64_t id;
    uint64_t value;
    uint32_t length;
    uint8_t type;
    uint8_t accept;
};


#endif //PACKET_H
