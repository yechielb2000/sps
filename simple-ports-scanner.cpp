#include <iostream>
#include "OptionsParser.hpp"



int main(int argc, char *argv[]) {
    const ScanConfig config = OptionsParser::parse(argc, argv);
    std::cout << "Scan started with provided configuration: " << std::endl;
    std::cout << config.toString() << std::endl;


    return 0;
}
