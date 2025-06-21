#pragma once

// Define DEBUG_LOGGING to enable logging
//#define DEBUG_LOGGING 

#ifdef DEBUG_LOGGING
    #define LOG_INFO(x) std::cout << x << std::endl
    #define LOG_ERROR(x) std::cerr << x << std::endl
#else
    #define LOG_INFO(x)  // No-op
    #define LOG_ERROR(x) // No-op
#endif