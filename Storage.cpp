#include "Storage.h"

#include <iostream>
#include <fstream>
#include <filesystem>
#include <sstream>

#include "configReader/ConfigReader.h"
#include "logger/log.h"

#include "UbjsonCpp/include/stream_writer.hpp"
#include "UbjsonCpp/include/stream_reader.hpp"

namespace ebmlio
{

namespace fs = std::filesystem;

constexpr auto FILE_TYPE = ".dat";

Storage::Storage()
    : config(std::make_unique<ConfigReader>())
{
    if (config->isCfgLoaded()) {
        LOG(config->getMaxFileSize());
        LOG(config->getStoragePath());

        checkStorageDir();
    }
}

Storage::~Storage() = default;

void Storage::m_save(TimeStamp time, const RawData& data)
{
    auto fileName = findFile(time);
    bool fileExists = fs::exists(fileName);

    auto file = std::filesystem::path (fileName);
    auto fileTimeStamp = fs::exists(fileName) ? timeFromString(file.stem()) : time;
    auto diff = time - fileTimeStamp;

    RawData obj;

    if (fileExists) {
        std::ifstream input;
        input.open(findFile(time), std::ios::binary);

        ubjson::StreamReader<decltype(input)> reader(input);
        obj = reader.getNextValue();
    }

    obj[std::to_string(diff)] = data;

    std::ofstream output;
    output.open(findFile(time), std::ios::binary);

    ubjson::StreamWriter<std::ostream> writer(output);
    auto result = writer.writeValue(obj);

    output.close();

    if(result.second)
        std::cout << "Successfully wrote: " << result.first << " bytes" << std::endl;
}

std::optional<RawData> Storage::m_load(TimeStamp time)
{
    auto fileName = findFile(time);
    auto file = std::filesystem::path (fileName);
    if (!fs::exists(fileName))
        return {};

    auto fileTimeStamp = timeFromString(file.stem());
    auto diff = time - fileTimeStamp;

    std::ifstream input;
    input.open(findFile(time), std::ios::binary);

    ubjson::StreamReader<decltype(input)> reader(input);
    RawData obj = reader.getNextValue();

    auto it = obj.find(std::to_string(diff));
    if (it != obj.end())
        return *it;

    return {};
}

void Storage::checkStorageDir()
{
    if (!fs::exists(config->getStoragePath()))
        fs::create_directory(config->getStoragePath());
}

std::string Storage::findFile(TimeStamp time)
{
    return config->getStoragePath() + "/" + "1590605908.dat";
}

std::string Storage::timeToString(TimeStamp time)
{
    std::stringstream ss;
    ss << time;
    return ss.str();
}

TimeStamp Storage::timeFromString(const std::string& data)
{
    std::istringstream stream( data );
    TimeStamp result;
    stream >> result;
    return result;
}

}
