#pragma once

#include <condition_variable>
#include <memory>
#include "option_parser.hpp"
#include "spdlog/logger.h"

class scanner {
    int active_threads = 0;
    std::condition_variable cv;
    ScanConfig config;
    std::mutex mutex;
    std::vector<int> open_ports{};
    std::shared_ptr<spdlog::logger> logger_;

    class SocketRAII {
    public:
        explicit SocketRAII(const int sock_fd) : sock_(sock_fd) {}
        ~SocketRAII();
        int get() const { return sock_; }
    private:
        int sock_;
    };

#ifdef _WIN32
    class WSAInitializer {
    public:
        WSAInitializer();
        ~WSAInitializer();
    private:
        bool initialized_ = false;
    };
#endif

    bool is_port_open(int port);

public:
    explicit scanner(const ScanConfig &config);
    ~scanner() = default;

    scanner(const scanner&) = delete;
    scanner& operator=(const scanner&) = delete;

    void scan_ports();
    void print_summary() const;
};