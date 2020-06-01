#pragma once

#include <string>

#include "dataTypes/RawData.h"

namespace storage
{

template<>
RawData serialize(const std::string& data);

template<>
RawData serialize(const double& data);

template<>
RawData serialize(const int& data);

template<>
RawData serialize(const bool& data);

template<>
std::optional<std::string> deserialize (const RawData& data);

template<>
std::optional<double> deserialize (const RawData& data);

template<>
std::optional<int> deserialize (const RawData& data);

template<>
std::optional<bool> deserialize (const RawData& data);

}
