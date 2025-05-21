#include <iostream>
#include <cxxopts.hpp>

#include "OptionsParser.hpp"


int main(int argc, char *argv[]) {
    Options opts = OptionsParser::parse(argc, argv);

    return 0;
}
