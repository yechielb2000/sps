#pragma once

#include <string>
#include <vector>
#include <cxxopts.hpp>

struct ScanConfig {
    std::string address;
    std::vector<int> ports;
    bool verbose = false;
    int threads = 10;
};

class OptionsParser {
public:
    static ScanConfig parse(int argc, char* argv[]);
};
