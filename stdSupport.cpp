
#include "stdSupport.h"

#include <string>

namespace storage
{

template<>
RawData serialize(const std::string &data)
{
    return data;
}

template<>
RawData serialize(const double& data)
{
    return data;
}

template<>
RawData serialize(const int& data)
{
    return data;
}

template<>
std::optional<std::string> deserialize(const RawData &data)
{
    return data.asString();
}

template<>
std::optional<double> deserialize (const RawData& data)
{
    return data.asFloat();
}

template<>
std::optional<int> deserialize (const RawData& data)
{
    return data.asInt();
}

template<>
std::optional<bool> deserialize (const RawData& data)
{
    return data.asBool();
}

}
