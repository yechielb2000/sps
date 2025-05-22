#include "OptionsParser.hpp"
#include <regex>
#include <sstream>
#include <iostream>
#include <stdexcept>

bool validate_ip(const std::string &ip) {
    const std::regex ip_pattern(
        R"(^(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)(\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)){3}$)"
    );
    return std::regex_match(ip, ip_pattern);
}

bool parse_ports(const std::string &ports_str, std::vector<int> &ports) {
    constexpr int MAX_PORT_NUMBER = 65535;
    ports.clear();
    auto dash_pos = ports_str.find('-');
    if (dash_pos != std::string::npos) {
        int start = std::stoi(ports_str.substr(0, dash_pos));
        int end = std::stoi(ports_str.substr(dash_pos + 1));
        if (start <= 0 || end <= 0 || start > MAX_PORT_NUMBER || end > MAX_PORT_NUMBER || start > end)
            return false;
        for (int p = start; p <= end; ++p)
            ports.push_back(p);
        return true;
    }

    std::stringstream ss(ports_str);
    std::string port_str;
    while (std::getline(ss, port_str, ',')) {
        try {
            int port = std::stoi(port_str);
            if (port <= 0 || port > MAX_PORT_NUMBER)
                return false;
            ports.push_back(port);
        } catch (...) {
            return false;
        }
    }
    return !ports.empty();
}

ScanConfig OptionsParser::parse(int argc, char *argv[]) {
    cxxopts::Options options("Port Scanner", "Simple port scanner");

    options.add_options()
            ("a,address", "IP address to scan", cxxopts::value<std::string>()->default_value("127.0.0.1"))
            ("p,ports", "Ports to scan (e.g. 22,80,443 or 20-25)",
             cxxopts::value<std::string>()->default_value("1-65535"))
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

    opts.address = result["address"].as<std::string>();
    if (!validate_ip(opts.address)) {
        throw std::invalid_argument("Invalid IP address format: " + opts.address);
    }

    if (!result.count("ports")) {
        throw std::invalid_argument("Ports must be specified with -p option");
    }

    const auto ports_str = result["ports"].as<std::string>();
    if (!parse_ports(ports_str, opts.ports)) {
        throw std::invalid_argument("Invalid ports format: " + ports_str);
    }

    opts.verbose = result["verbose"].as<bool>();
    opts.threads = result["threads"].as<int>();
    opts.timeout = result["timeout"].as<int>();

    return opts;
}
