#pragma once

#include "dataTypes/TimeStamp.h"
#include "dataTypes/RawData.h"

#include <optional>
#include <string>

namespace ebmlio
{

class ConfigReader;

class Storage {

public:
    Storage();
    ~Storage();

    template<typename T>
    void save(TimeStamp time, const T& data) {
        m_save(time, toRawData<T>(data));
    }

    template<typename T>
    std::optional<T> load(TimeStamp time) {
        auto optRaw = m_load(time);
        return optRaw.has_value() ? fromRawData<T>(optRaw.value()) : std::optional<T>();
    }

private:
    void m_save(TimeStamp time, const RawData& data);
    std::optional<RawData> m_load(TimeStamp time);

    void checkStorageDir();
    std::string findFile(TimeStamp time);

    std::string timeToString(TimeStamp time);
    TimeStamp timeFromString(const std::string& data);

private:
    std::unique_ptr<ConfigReader> config;
};

}

