#pragma once

#include <string>

#include "dataTypes/RawData.h"

namespace storage
{

template<>
RawData serialize(const unsigned int& data);

template<>
RawData serialize(const int8_t& data);

template<>
RawData serialize(const uint8_t& data);

template<>
RawData serialize(const int16_t& data);

template<>
RawData serialize(const uint16_t& data);

template<>
std::optional<unsigned int> deserialize (const RawData& data);

template<>
std::optional<unsigned long long> deserialize (const RawData& data);

template<>
std::optional<int8_t> deserialize (const RawData& data);

template<>
std::optional<uint8_t> deserialize (const RawData& data);

template<>
std::optional<int16_t> deserialize (const RawData& data);

template<>
std::optional<uint16_t> deserialize (const RawData& data);

}
