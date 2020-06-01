#pragma once

#include "dataTypes/TimeStamp.h"
#include "dataTypes/RawData.h"

#include <optional>
#include <string>
#include <string_view>
#include <experimental/propagate_const>

namespace storage
{

class StoragePrivate;

class Storage {
    std::experimental::propagate_const<std::unique_ptr<StoragePrivate>> pImpl;

public:
    Storage();
    ~Storage();

    ///< Single save
    template<typename T>
    void save(TimeStamp time, const T& data) {
        m_save(time, serialize<T>(data));
    }

    ///< Single load
    template<typename T>
    std::optional<T> load(TimeStamp time) {
        auto optRaw = m_load(time);
        return optRaw.has_value() ? deserialize<T>(optRaw.value()) : std::optional<T>();
    }

    template<typename T>
    std::map<TimeStamp, T> loadRange(TimeStamp from, TimeStamp to) {
        std::map<TimeStamp, T> result;

        for (const auto& data : m_loadRange(from, to)) {
            result[data.first] = deserialize<T>(data.second);
        }

        return result;
    }

    ///< Better for multiple saves but you should embrace it with beginInsert() and endInsert()
    template<typename T>
    void insert(TimeStamp time, const T& data) {
        m_insert(time, serialize<T>(data));
    }

    ///< For multiple saves
    void beginInsert(TimeStamp time);
    void endInsert();

private:
    void m_save(TimeStamp time, const RawData& data);
    std::optional<RawData> m_load(TimeStamp time);
    std::map<TimeStamp, RawData> m_loadRange(TimeStamp from, TimeStamp to);

    void m_insert(TimeStamp time, const RawData& data);
};

}

