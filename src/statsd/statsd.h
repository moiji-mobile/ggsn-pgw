#pragma once

#include <string>
#include <chrono>

class TestStatsD;
class StatsDTimer;

/**
 * I handle statsd output and similiar things.
 */
class StatsDController {
    friend class TestStatsD;
public:
    StatsDController();
    ~StatsDController();

    StatsDController(const StatsDController&) = delete;
    StatsDController& operator=(const StatsDController&) = delete;

    /** bind a socket and look up the hostname */
    void connect(const std::string& hostOrIp, int port);

    void time(const std::string& variable, unsigned int ms);
    void incr(const std::string& variable);
    void flush();

    StatsDTimer timeFunction(const std::string& name);

private:
    void queue(const std::string& var, const std::string& txt, const std::string& type);
    int m_fd;
    std::string m_output;
};

class StatsDTimer {
public:
    StatsDTimer(StatsDController& ctrl, const std::string& name);
    ~StatsDTimer();

private:
    StatsDController& m_statsd;
    std::string m_name;
    std::chrono::time_point<std::chrono::system_clock> m_startTime;
};
