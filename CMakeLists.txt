cmake_minimum_required(VERSION 3.16)
project(ebmlio)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(json)
add_subdirectory(UbjsonCpp)

find_package(EBML 1.3.9 REQUIRED)
find_package(Matroska CONFIG REQUIRED)

add_library(ebmlio SHARED
        Storage.cpp
        Storage.h
        configReader/ConfigReader.cpp
        configReader/ConfigReader.h

        dataTypes/RawData.h
        dataTypes/stdSupport.h
        dataTypes/stdSupport.cpp
        dataTypes/TimeStamp.h
        dataTypes/FileInfo.h

        logger/log.h

        private/StoragePrivate.cpp
        private/StoragePrivate.h
        )

target_link_libraries(ebmlio PUBLIC nlohmann_json::nlohmann_json)
target_link_libraries(ebmlio PUBLIC EBML::ebml)
target_link_libraries(ebmlio PRIVATE Matroska::matroska)
target_link_libraries(ebmlio PUBLIC UbjsonCpp)
