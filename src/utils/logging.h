#pragma once

#include <string>
#include <iostream>
#include <sstream>
#include <iomanip>

#include <syslog.h>
#include <stdint.h>

enum {
    LOG_GTP,
    LOG_STATSD,
};

extern bool enable_syslog;

static inline void logDebug(int /*area*/, const std::string& /*msg*/)
{
    /* don't print right now */
}

static inline void logWarning(int /*area*/, const std::string& msg)
{
    if (enable_syslog)
        return syslog(LOG_WARNING, "%s", msg.c_str());
    std::cout << "WARNING: " << msg << std::endl;
}

static inline void logError(int /*area*/, const std::string& msg)
{
    if (enable_syslog)
        return syslog(LOG_ERR, "%s", msg.c_str());
    std::cout << "ERROR: " << msg << std::endl;
}

static inline std::string hexstring(const uint8_t *data, size_t len)
{
    std::stringstream stream;
    stream << std::hex;

    for (size_t i = 0; i < len; ++i)
        stream << std::setw(2) << std::setfill('0') << (int) data[i];
    return stream.str();
}
