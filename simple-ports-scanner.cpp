#include "OptionsParser.hpp"
#include "logger.h"
#include "Scanner.h"

int main(int argc, char *argv[]) {
    setup_logger();

    const ScanConfig config = OptionsParser::parse(argc, argv);

    const auto logger = get_logger();


    logger->info("Scan started with provided configuration:\n {}", config.toString());
    logger->info("Scanning...");

    Scanner scanner(config);
    scanner.scan_ports();
    scanner.print_summary();

    return 0;
}
