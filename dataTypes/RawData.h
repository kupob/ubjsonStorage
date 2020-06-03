#pragma once

#include <optional>
#include <string>

#include "../UbjsonCpp/include/value.hpp"

namespace storage
{

typedef ubjson::Value RawData;

template<typename T>
std::optional<T> deserialize(const RawData& data)
{
    return data;
}

template<typename T>
RawData serialize(const T& data)
{
    return data;
}

}
