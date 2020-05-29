
#include "ConfigReader.h"

#include <nlohmann/json.hpp>

#include "../logger/log.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

namespace storage
{

constexpr auto CFG_PATH = "ioconfig.cfg";

#ifdef __linux__
constexpr auto STORAGE_PATH_DEFAULT = "/tmp/storage/";
#elif _WIN32
constexpr auto STORAGE_PATH_DEFAULT = "%temp%/storage/";
#elif __APPLE__
constexpr auto STORAGE_PATH_DEFAULT = "/tmp/storage/";
#endif

constexpr auto MAX_FILE_SIZE_DEFAULT = 1000;

ConfigReader::ConfigReader()
{
    if (!read()) {
        if (!createDefault()) {
            LOG("Error writing default config");
        }
        else {
            LOG("Default config created");
        }
    }
}

ConfigReader::~ConfigReader() = default;

bool ConfigReader::read()
{
    std::ifstream f(CFG_PATH);
    std::string cfgContents;
    if(f) {
        std::ostringstream ss;
        ss << f.rdbuf();
        cfgContents = ss.str();

        return parseCfgContents(cfgContents);;
    }
    else {
        LOG("Could not find cfg file");
    }

    return false;
}

bool ConfigReader::createDefault()
{
    nlohmann::json config;
    config["max_file_size"] = MAX_FILE_SIZE_DEFAULT;
    config["storage_path"] = STORAGE_PATH_DEFAULT;

    std::ofstream o(CFG_PATH);
    o << std::setw(4) << config << std::endl;
    o.close();

    m_maxFileSize = MAX_FILE_SIZE_DEFAULT;
    m_storagePath = STORAGE_PATH_DEFAULT;

    return true;
}

bool ConfigReader::parseCfgContents(std::string_view data)
{
    using json = nlohmann::json;

    json contents = json::parse(data);

    if (contents.count("max_file_size"))
        m_maxFileSize = contents["max_file_size"];

    if (contents.count("storage_path"))
        m_storagePath = contents["storage_path"];

    cfgLoaded = checkCfgCompleteness();

    return cfgLoaded;
}

bool ConfigReader::checkCfgCompleteness()
{
    return m_maxFileSize
        && !m_storagePath.empty()
        ;
}

} // namespace
