#ifndef SCANNER_H
#define SCANNER_H

#include <condition_variable>
#include <memory>
#include "OptionsParser.hpp"
#include "spdlog/logger.h"

class Scanner {
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
    explicit Scanner(const ScanConfig &config);
    ~Scanner() = default;

    Scanner(const Scanner&) = delete;
    Scanner& operator=(const Scanner&) = delete;

    void scan_ports();
    void print_summary() const;
};

#endif