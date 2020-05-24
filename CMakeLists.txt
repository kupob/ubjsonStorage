cmake_minimum_required(VERSION 3.16)
project(ebmlio)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(json)
#add_subdirectory(libebml)
add_subdirectory(dataTypes)

add_library(ebmlio SHARED
        Storage.cpp
        Storage.h
        configReader/ConfigReader.cpp
        configReader/ConfigReader.h

        dataTypes/RawData.h
        dataTypes/stdSupport.h
        dataTypes/stdSupport.cpp
        dataTypes/TimeStamp.h

        logger/log.h)

target_link_libraries(ebmlio PRIVATE nlohmann_json::nlohmann_json)
#target_link_libraries(ebmlio PRIVATE libebml)
