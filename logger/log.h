
#pragma once

#if LOG_LEVEL > 1
    #define LOGERROR(msg) \
        std::cout << "ERROR: " << msg << std::endl
    #define LOG(msg) \
        std::cout << "LOG: " << msg << std::endl
#elif LOG_LEVEL > 0
    #define LOGERROR(msg) \
        std::cout << "ERROR: " << msg << std::endl
    #define LOG(msg)
#else
    #define LOGERROR(msg)
    #define LOG(x)
#endif

