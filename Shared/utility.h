#ifndef UTILITY_H
#define UTILITY_H
#include <boost/circular_buffer.hpp>

namespace utility
{
    inline bool parse_message(boost::circular_buffer<char>& buffer, std::vector<char>& packet_data)
    {
        if (buffer.size() < 4)
        {
            return false;
        }

        const uint32_t packet_len = ((buffer[0] << 24) | (buffer[1] << 16) | (buffer[2] << 8) | buffer[3]) + 4;

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

    template <typename T>
    void append_bytes(std::vector<char>& buffer, const T& value)
    {
        static_assert(std::is_trivially_copyable_v<T>, "Type must be trivially copyable");
        const char * ptr = reinterpret_cast<const char*>(&value);
        buffer.insert(buffer.end(), ptr, ptr + sizeof(T));
    }

    inline uint16_t bytes_to_u16int(const int start, const int end, const std::vector<char>& data)
    {
        uint16_t num = 0;
        for (int i = start; i < end; ++i)
        {
            num = static_cast<uint16_t>(data[i] << ((i - start) * 2)) | num;
        }
        return num;
    }

    inline uint32_t bytes_to_u32int(const int start, const int end, const std::vector<char>& data)
    {
        uint32_t num = 0;
        for (int i = start; i <= end; ++i)
        {
            num = static_cast<uint32_t>(data[i] << ((i - start) * 4)) | num;
        }
        return num;
    }

    inline uint64_t bytes_to_u64int(const int start, const int end, const std::vector<char>& data)
    {
        uint64_t num = 0;
        for (int i = start; i <= end; ++i)
        {
            num = static_cast<uint64_t>(data[i] << ((i - start) * 8)) | num;
        }
        return num;
    }
}

#endif //UTILITY_H
