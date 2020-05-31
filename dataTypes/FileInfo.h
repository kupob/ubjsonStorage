
#pragma once

#include <string>

#include "TimeStamp.h"

namespace storage
{

struct FileInfo
{
    std::string name;
    TimeStamp time = 0;

    int32_t dataCount = 0;

    bool isValid() const noexcept { return !name.empty() && time != 0; }
};

}
