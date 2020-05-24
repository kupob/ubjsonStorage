#pragma once

#include <optional>
#include <string>

struct RawData {

};

template<typename T>
std::optional<T> fromRawData(const RawData& data);

template<typename T>
RawData toRawData(const T& data);
