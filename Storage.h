#pragma once

#include "dataTypes/TimeStamp.h"
#include "dataTypes/RawData.h"

#include <optional>

namespace ebmlio
{

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
        return fromRawData<T>(m_load(time));
    }

private:
    void m_save(TimeStamp time, const RawData& data);
    RawData m_load(TimeStamp time);
};

}

