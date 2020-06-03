
#include "stdSupport.h"

#include <string>

namespace storage
{

template<>
RawData serialize(const unsigned int& data)
{
    return RawData {(unsigned long long) data};
}

template<>
RawData serialize(const int8_t& data)
{
    return RawData {(int) data};
}

template<>
RawData serialize(const uint8_t& data)
{
    return RawData {(unsigned long long) data};
}

template<>
RawData serialize(const int16_t& data)
{
    return RawData {(int) data};
}

template<>
RawData serialize(const uint16_t& data)
{
    return RawData {(unsigned long long) data};
}

template<>
std::optional<unsigned int> deserialize(const RawData& data)
{
    return data.asUint64();
}

template<>
std::optional<unsigned long long> deserialize (const RawData& data)
{
    return data.asUint64();
}

template<>
std::optional<int8_t> deserialize (const RawData& data)
{
    return data.asInt();
}

template<>
std::optional<uint8_t> deserialize (const RawData& data)
{
    return data.asUint();
}

template<>
std::optional<int16_t> deserialize (const RawData& data)
{
    return data.asInt();
}

template<>
std::optional<uint16_t> deserialize (const RawData& data)
{
    return data.asUint();
}

}
