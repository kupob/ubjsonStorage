cmake_minimum_required(VERSION 3.16)
project(ubjsonStorage)

set(CMAKE_CXX_STANDARD 17)

#add_compile_definitions(LOG_LEVEL=1)

add_subdirectory(json)
add_subdirectory(UbjsonCpp)

add_library(ubjsonStorage SHARED
        Storage.cpp
        Storage.h
        configReader/ConfigReader.cpp
        configReader/ConfigReader.h

        dataTypes/RawData.h
        stdSupport.h
        stdSupport.cpp
        dataTypes/TimeStamp.h
        dataTypes/FileInfo.h

        logger/log.h

        private/StoragePrivate.cpp
        private/StoragePrivate.h
        )

target_link_libraries(ubjsonStorage PUBLIC nlohmann_json::nlohmann_json)
target_link_libraries(ubjsonStorage PUBLIC UbjsonCpp)
