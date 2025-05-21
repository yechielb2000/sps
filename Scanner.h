#ifndef SCANNER_H
#define SCANNER_H

#include <condition_variable>
#include "OptionsParser.hpp"

class Scanner {
    int active_threads = 0;
    std::condition_variable cv;

    int open_ports_count = 0;
    int total_ports_count = 0;
    ScanConfig config;
    std::mutex mutex;

    void sendRequest(const std::string &address, int port, int timeout);

public:
    explicit Scanner(const ScanConfig &config);

    void scanPorts();

    void printResult(const std::string &address, int port, bool is_open);

    void printSummary();
};


#endif //SCANNER_H
