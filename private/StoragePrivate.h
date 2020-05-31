
#pragma once

#include "../dataTypes/TimeStamp.h"
#include "../dataTypes/FileInfo.h"
#include "../dataTypes/RawData.h"

namespace storage
{

class Storage;
class ConfigReader;

class StoragePrivate
{
public:
    StoragePrivate();
    ~StoragePrivate();

private:
    void save(TimeStamp time, const RawData& data);
    std::optional<RawData> load(TimeStamp time);
    std::map<TimeStamp, RawData> loadRange(TimeStamp from, TimeStamp to);
    void insert(TimeStamp time, const RawData& data);

    void beginInsert(TimeStamp time);
    void endInsert();

    void beginRead(TimeStamp time);
    void endRead();

    void checkStorageDir();
    void analyzeStorage();
    void initNewStorageFile(TimeStamp time, std::string_view fileName);

    FileInfo findFileForRead(TimeStamp time);
    FileInfo findFileForWrite(TimeStamp time);
    std::string generateFileName(TimeStamp time);

    std::string timeToString(TimeStamp time);
    TimeStamp timeFromString(const std::string& data);

private:
    std::unique_ptr<ConfigReader> m_config;
    std::map<TimeStamp, FileInfo> m_storageFiles;

    RawData m_writeObj;
    FileInfo m_writeFile;

    RawData m_readObj;
    FileInfo m_readFile;

private:
    friend class Storage;
};

}
