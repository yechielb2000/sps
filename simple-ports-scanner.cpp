#include <iostream>
#include "OptionsParser.hpp"

#include "Scanner.h"

int main(int argc, char *argv[]) {
    const ScanConfig config = OptionsParser::parse(argc, argv);
    std::cout << "Scan started with provided configuration: " << std::endl;
    std::cout << config.toString() << std::endl;

    std::cout << std::endl;
    std::cout << "Scanning..." << std::endl;

    Scanner scanner(config);
    scanner.scanPorts();
    scanner.print_summary();

    return 0;
}
