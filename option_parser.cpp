#include "option_parser.hpp"
#include <regex>
#include <sstream>
#include <iostream>
#include <stdexcept>

#include "logger.hpp"
#include "spdlog/logger.h"


namespace {
    void validate_ip(const std::string &ip) {
        const auto logger = get_logger();
        logger->debug("Validating IP address: {}", ip);
        const std::regex ip_pattern(
            R"(^(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)(\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)){3}$)"
        );
        if (std::regex_match(ip, ip_pattern)) {
            logger->debug("IP address is valid");
        } else {
            logger->error("Invalid IP address: {}", ip);
        }
    }

    void parse_ports(const std::string &ports_str, std::vector<int> &ports) {
        const auto logger = get_logger();
        logger->debug("Parsing ports: {}", ports_str);
        constexpr int MAX_PORT_NUMBER = 65535;
        ports.clear();
        if (const auto dash_pos = ports_str.find('-'); dash_pos != std::string::npos) {
            const int start = std::stoi(ports_str.substr(0, dash_pos));
            const int end = std::stoi(ports_str.substr(dash_pos + 1));
            if (start <= 0 || end <= 0 || start > MAX_PORT_NUMBER || end > MAX_PORT_NUMBER || start > end)
                logger->error("Invalid ports format: {}", ports_str);
            for (int p = start; p <= end; ++p)
                ports.push_back(p);
        }

        std::stringstream ss(ports_str);
        std::string port_str;
        while (std::getline(ss, port_str, ',')) {
            int port = std::stoi(port_str);
            if (port <= 0 || port > MAX_PORT_NUMBER)
                logger->error("Port {} is not between 1-65535", port);
            ports.push_back(port);
        }
    }
}


ScanConfig option_parser::parse(int argc, char *argv[]) {
    const auto logger = get_logger();
    cxxopts::Options options("Port Scanner", "Simple port scanner");

    options.add_options()
            ("a,address", "IP address to scan", cxxopts::value<std::string>()->default_value("127.0.0.1"))
            ("p,ports", "Ports to scan (e.g. 22,80,443 or 20-25)", cxxopts::value<std::string>()->default_value("1-65535"))
            ("v,verbose", "Verbose output", cxxopts::value<bool>()->default_value("false"))
            ("t,threads", "Max threads", cxxopts::value<int>()->default_value("10"))
            ("m,timeout", "Timeout in seconds", cxxopts::value<int>()->default_value("12"))
            ("h,help", "Print usage");

    const auto result = options.parse(argc, argv);

    if (result.count("help")) {
        std::cout << options.help() << std::endl;
        exit(0);
    }

    ScanConfig opts;

    opts.verbose = result["verbose"].as<bool>();
    if (opts.verbose) {
        logger->set_level(spdlog::level::debug);
    }

    opts.address = result["address"].as<std::string>();
    validate_ip(opts.address);

    const auto ports_str = result["ports"].as<std::string>();
    parse_ports(ports_str, opts.ports);

    opts.threads = result["threads"].as<int>();
    opts.timeout = result["timeout"].as<int>();

    return opts;
}
