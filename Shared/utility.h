#ifndef UTILITY_H
#define UTILITY_H
#include <boost/circular_buffer.hpp>

namespace utility
{
    inline bool parse_message(boost::circular_buffer<uint8_t>& buffer, std::vector<uint8_t>& packet_data)
    {
        if (buffer.size() < 4)
        {
            return false;
        }

        const uint32_t packet_len = (buffer[0] << 24) | (buffer[1] << 16) | (buffer[2] << 8) | buffer[3] + 4;

        if(buffer.size()< packet_len )
        {
            return false;
        }

        for (int i = 0; i < packet_len; ++i)
        {
            packet_data.push_back(buffer.front());
            buffer.pop_front();
        }

        return true;
    }
}

#endif //UTILITY_H
