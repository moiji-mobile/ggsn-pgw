#include "statsd.h"

#include "utils/logging.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>

StatsDTimer::StatsDTimer(StatsDController &statsd, const std::string &name)
    : m_statsd(statsd)
    , m_name(name)
{
    m_startTime = std::chrono::system_clock::now();
}

StatsDTimer::~StatsDTimer()
{
    auto end = std::chrono::system_clock::now();
    auto dur = std::chrono::duration_cast<std::chrono::milliseconds> (end - m_startTime);
    m_statsd.time(m_name, dur.count());
}

StatsDController::StatsDController()
    : m_fd(-1)
{}

StatsDController::~StatsDController()
{
    if (m_fd >= 0)
        close(m_fd);
    m_output.reserve(2048);
}

void StatsDController::connect(const std::string& hostOrIp, int port)
{
    struct addrinfo hints, *res, *res0;
    std::string portbuf = std::to_string(port);

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = PF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_flags = AI_NUMERICSERV;
    hints.ai_protocol = IPPROTO_UDP;
    int error = getaddrinfo(hostOrIp.c_str(), portbuf.c_str(), &hints, &res0);
    if (error) {
        logError(LOG_STATSD, "Failed to query: " + std::to_string(errno));
        return;
    }

    int fd = -1;
    for (res = res0; res; res = res->ai_next) {
        fd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
        if (fd < 0) {
            logError(LOG_STATSD, "Failed to create socket for " +
                     std::to_string(res->ai_family) + " " +
                     std::to_string(res->ai_socktype) + " " +
                     std::to_string(res->ai_protocol));
            continue;
        }

        if (::connect(fd, res->ai_addr, res->ai_addrlen) < 0) {
            logError(LOG_STATSD, "Failed to connect StatsD socket: " + std::to_string(errno) + " " + strerror(errno));
            close(fd);
            fd = -1;
            continue;
        }

        m_fd = fd;
        break;
    }
    freeaddrinfo(res0);
}

void StatsDController::time(const std::string& var, unsigned int ms)
{
    queue(var, std::to_string(ms), "ms");
}

void StatsDController::incr(const std::string& var)
{
    queue(var, "1", "c");
}

void StatsDController::queue(const std::string& var, const std::string& txt, const std::string& type)
{
    if (m_fd < 0)
        return;
    if (!m_output.empty())
        m_output += "\n";
    m_output += var + ":" + txt + "|" + type;
}

void StatsDController::flush()
{
    if (m_output.empty())
        return;
    write(m_fd, m_output.c_str(), m_output.size());
    m_output.clear();
}

StatsDTimer StatsDController::timeFunction(const std::string& name)
{
    return StatsDTimer(*this, name);
}
