#pragma once

#include <string>
#include <vector>
#include <cxxopts.hpp>

struct Options {
    std::string address;
    std::vector<int> ports;
    bool verbose = false;
    int threads = 10;
};

class OptionsParser {
public:
    static Options parse(int argc, char* argv[]);
};
