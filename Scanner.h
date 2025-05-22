#ifndef SCANNER_H
#define SCANNER_H

#include <condition_variable>
#include "OptionsParser.hpp"
#include "spdlog/logger.h"

class Scanner {
    int active_threads = 0;
    std::condition_variable cv;
    ScanConfig config;
    std::mutex mutex;
    std::vector<int> open_ports{};
    std::shared_ptr<spdlog::logger> logger_;


    bool is_port_open(int port);

public:
    explicit Scanner(const ScanConfig &config);

    void scan_ports();

    void print_summary() const;
};


#endif
