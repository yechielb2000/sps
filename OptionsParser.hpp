#pragma once

#include <string>
#include <vector>
#include <cxxopts.hpp>

struct ScanConfig {
    std::string address;
    std::vector<int> ports;
    bool verbose = false;
    int threads = 10;

    std::string toString() const {
        std::string config =
                "address: " + address + "\n"
                "ports: " + std::to_string(ports.size()) + "\n"
                "verbose: " + std::to_string(verbose) + "\n"
                "maximum threads: " + std::to_string(threads) + "\n";
        return config;
    }
};

class OptionsParser {
public:
    static ScanConfig parse(int argc, char *argv[]);
};
