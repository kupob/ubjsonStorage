#include "Storage.h"

#include <iostream>

#include "configReader/ConfigReader.h"
#include "logger/log.h"

namespace ebmlio
{

Storage::Storage()
{
    ConfigReader reader;

    LOG(reader.getMaxFileSize());
    LOG(reader.getStoragePath());
}

Storage::~Storage() = default;

void Storage::m_save(TimeStamp time, const RawData& data)
{

}

RawData Storage::m_load(TimeStamp time)
{
    RawData result;

    return result;
}

}
