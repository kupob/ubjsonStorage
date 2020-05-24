
#include "stdSupport.h"

template<>
RawData toRawData(const std::string& data)
{
    RawData result;
    return result;
}

template<>
std::optional<std::string> fromRawData (const RawData& data)
{
    std::string result;
    return result;
}
