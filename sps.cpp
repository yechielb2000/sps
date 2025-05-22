#include "option_parser.hpp"
#include "logger.hpp"
#include "scanner.hpp"

int main(int argc, char *argv[]) {
    setup_logger();

    const ScanConfig config = option_parser::parse(argc, argv);

    const auto logger = get_logger();


    logger->info("Scan started with provided configuration:\n{}", config.toString());
    logger->info("Scanning...");

    scanner scanner(config);
    scanner.scan_ports();
    scanner.print_summary();

    return 0;
}
