#ifndef SCANNER_H
#define SCANNER_H

#include <condition_variable>
#include "OptionsParser.hpp"

class Scanner {
    int active_threads = 0;
    std::condition_variable cv;
    int open_ports_count = 0;
    ScanConfig config;
    std::mutex mutex;
    std::vector<int> open_ports{};


    bool isPortOpen(int port) const;

public:
    explicit Scanner(const ScanConfig &config);

    void scanPorts();

    void printSummary();
};


#endif //SCANNER_H
