
#include "StoragePrivate.h"

#include <iostream>
#include <fstream>
#include <filesystem>
#include <sstream>

#include "../configReader/ConfigReader.h"
#include "../logger/log.h"

#include "../UbjsonCpp/include/stream_writer.hpp"
#include "../UbjsonCpp/include/stream_reader.hpp"

namespace storage
{

namespace fs = std::filesystem;

constexpr auto FILE_TYPE = ".dat";

StoragePrivate::StoragePrivate()
    : m_config(std::make_unique<ConfigReader>())
{
    if (m_config->isCfgLoaded()) {
        LOG(m_config->getMaxFileSize());
        LOG(m_config->getStoragePath());

        checkStorageDir();
        analyzeStorage();
    }
}

StoragePrivate::~StoragePrivate() = default;

void StoragePrivate::save(TimeStamp time, const RawData& data)
{
    LOG("SAVE " << time);

    beginInsert(time);

    insert(time, data);

    endInsert();
}

std::optional<RawData> StoragePrivate::load(TimeStamp time)
{
    LOG("LOAD " << time);
    beginRead(time);
    if (m_readFile.isValid()) {
        LOG("FILE FOUND " << m_readFile.name);
        auto diff = time - m_readFile.time;

        LOG("STORAGE SIZE " << m_readObj.size());

        auto it = m_readObj.find(std::to_string(diff));
        if (it != m_readObj.end()) {
            auto value = *it;
            endRead();
            return value;
        }
    }

    endRead();
    return {};
}

std::map<TimeStamp, RawData> StoragePrivate::loadRange(TimeStamp from, TimeStamp to)
{
    std::map<TimeStamp, RawData> result;

    beginRead(from);
    if (m_readFile.isValid()) {
        auto diffFrom = from - m_readFile.time;
        auto diffTo = to - m_readFile.time;

        auto keys = m_readObj.keys();

        std::sort(keys.begin(), keys.end(), [this](const auto& key1, const auto& key2) {
            return std::stol(key1) < std::stol(key2);
        });

        long diff {0};
        TimeStamp time {0};

        for (const auto& key : keys) {
            diff = std::stol(key);
            if (diffFrom < diff && diff < diffTo) {
                time = m_readFile.time + diff;
                result[time] = m_readObj[diff];
            }
            else if (diff > diffTo) {
                ///< we finised, return
                endRead();
                return result;
            }
        }

        auto subResult = loadRange(time, to);
        result.insert(subResult.begin(), subResult.end());
    }

    endRead();
    return result;
}

void StoragePrivate::insert(TimeStamp time, const RawData& data)
{
    if (m_writeObj.size() >= m_config->getMaxFileSize()) {
        endInsert();
        beginInsert(time);
    }

    auto diff = time - m_writeFile.time;
    m_writeObj[std::to_string(diff)] = data;
}

void StoragePrivate::beginInsert(TimeStamp time)
{
    m_writeFile = findFileForWrite(time);
    bool fileExists = fs::exists(m_writeFile.name);

    auto file = std::filesystem::path (m_writeFile.name);

    if (fileExists) {
        std::ifstream input;
        input.open(m_writeFile.name, std::ios::binary);

        ubjson::StreamReader<decltype(input)> reader(input);
        m_writeObj = reader.getNextValue();
    }
    else {
        initNewStorageFile(time, m_writeFile.name);
        m_writeObj = {};
    }
}

void StoragePrivate::endInsert()
{
    std::ofstream output;
    output.open(m_writeFile.name, std::ios::binary);

    ubjson::StreamWriter<std::ostream> writer(output);
    auto result = writer.writeValue(m_writeObj);

    output.close();

    if(result.second) {
        LOG("Successfully wrote: " << result.first << " bytes");
        LOG("File storage now has " << m_writeObj.size() << " entities");

        m_storageFiles[m_writeFile.time].dataCount = m_writeObj.size();
        m_writeObj = {};
    }
}

void StoragePrivate::beginRead(TimeStamp time)
{
    m_readFile = findFileForRead(time);
    if (!m_readFile.isValid())
        return;

    auto file = std::filesystem::path (m_readFile.name);
    if (!fs::exists(m_readFile.name))
        return;

    std::ifstream input;
    input.open(m_readFile.name, std::ios::binary);

    ubjson::StreamReader<decltype(input)> reader(input);
    m_readObj = reader.getNextValue();
}

void StoragePrivate::endRead()
{
    m_readFile = {};
    m_readObj = {};
}

void StoragePrivate::checkStorageDir()
{
    if (!fs::exists(m_config->getStoragePath()))
        fs::create_directories(m_config->getStoragePath());
}

void StoragePrivate::analyzeStorage()
{
    m_storageFiles.clear();

    for(auto& entry: fs::directory_iterator(m_config->getStoragePath())) {
        auto file = fs::path(entry.path());
        if (file.extension() == FILE_TYPE) {
            auto fileTime = timeFromString(file.stem());
            m_storageFiles[fileTime] = {entry.path(), fileTime, 0 /* TODO */};
        }
    }
}

void StoragePrivate::initNewStorageFile(TimeStamp time, std::string_view fileName)
{
    m_storageFiles[time] = {fileName.data(), time, 0};
}

FileInfo StoragePrivate::findFileForRead(TimeStamp time)
{
    if (m_storageFiles.empty()) {
        ///< there is no storage files at all
        return {};
    }

    auto it = m_storageFiles.upper_bound(time);
    if (it == m_storageFiles.begin()) {
        ///< the first storage file has newer timestamp, so the data is in the past
        return {};
    }

    return (--it)->second;
}

FileInfo StoragePrivate::findFileForWrite(TimeStamp time)
{
    if (m_storageFiles.empty()) {
        ///< there is no storage files at all
        return {generateFileName(time), time, 0};
    }

    auto it = m_storageFiles.upper_bound(time);
    if (it->second.time == time) {
        return it->second;
    }

    if (it == m_storageFiles.begin()) {
        ///< the first storage file has newer timestamp, so the data is in the past
        return {generateFileName(time), time, 0};
    }

    auto fileInfo = (--it)->second;
    if (fileInfo.dataCount >= m_config->getMaxFileSize()) {
        ///< the file is full, so generate new one
        return {generateFileName(time), time, 0};
    }

    return fileInfo;
}

std::string StoragePrivate::generateFileName(TimeStamp time)
{
    return m_config->getStoragePath() + "/" + timeToString(time) + FILE_TYPE;
}

std::string StoragePrivate::timeToString(TimeStamp time)
{
    std::stringstream ss;
    ss << time;
    return ss.str();
}

TimeStamp StoragePrivate::timeFromString(const std::string& data)
{
    std::istringstream stream( data );
    TimeStamp result;
    stream >> result;
    return result;
}


}
