#include "Scanner.h"
#include <iostream>
#include <mutex>
#include <thread>

#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0600
#endif

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
typedef int socklen_t;
#else
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#endif

#include "logger.h"


Scanner::Scanner(const ScanConfig &config) : config(config) {
    this->config = config;
    this->logger_ = get_logger();
}

void Scanner::scan_ports() {
    std::vector<std::thread> thread_pool;
    const int maximum_threads = this->config.threads;
    const std::string address = this->config.address;
    const bool verbose = this->config.verbose; {
        std::unique_lock cout_lock(this->mutex);
        for (int port: this->config.ports) {
            while (this->active_threads >= maximum_threads) {
                cv.wait(cout_lock, [this, maximum_threads] {
                    return this->active_threads < maximum_threads;
                });
            }

            this->active_threads++;
            if (verbose) {
                logger_->debug("Scanning {}:{}", address, port);
            }

            thread_pool.emplace_back([this, address, port, verbose] {
                const bool is_open = this->is_port_open(port);
                std::lock_guard lock(this->mutex);

                if (is_open) {
                    logger_->info("Found open port {}:{}", address, port);
                    this->open_ports.push_back(port);
                } else if (verbose) {
                    logger_->debug("Port is closed {}:{}", address, port);
                }
                this->active_threads--;
                cv.notify_all();
            });
        }
    }

    for (auto &thread: thread_pool) {
        thread.join();
    }
}

bool Scanner::is_port_open(const int port) {
    const std::string address = this->config.address;
    const int timeout = this->config.timeout;

#ifdef _WIN32
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
#endif

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        return false;
    }

#ifdef _WIN32
    u_long mode = 1;
    ioctlsocket(sock, FIONBIO, &mode);
#else
            fcntl(sock, F_SETFL, O_NONBLOCK);
#endif

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    inet_pton(AF_INET, address.c_str(), &addr.sin_addr);

    connect(sock, reinterpret_cast<sockaddr *>(&addr), sizeof(addr));

    fd_set writefds;
    FD_ZERO(&writefds);
    FD_SET(sock, &writefds);

    timeval tv{};
    tv.tv_sec = timeout;
    tv.tv_usec = (timeout % 1000) * 1000;

    const int res = select(sock + 1, nullptr, &writefds, nullptr, &tv);
    bool is_open = false;

    if (res > 0) {
        int so_error = 0;
        socklen_t len = sizeof(so_error);
        getsockopt(sock, SOL_SOCKET, SO_ERROR, reinterpret_cast<char *>(&so_error), &len);
        is_open = so_error == 0;
    }

#ifdef _WIN32
    closesocket(sock);
    WSACleanup();
#else
            close(sock);
#endif

    return is_open;
}

void Scanner::print_summary() const {
    const std::vector<int>::size_type open_ports = this->open_ports.size();
    std::cout << std::endl;
    std::cout << "----------------------------------" << std::endl;
    std::cout << "Found " << open_ports << " Open Ports!" << std::endl;
    for (size_t i = 0; i < open_ports; ++i) {
        std::cout << "port: " << this->open_ports[i] << std::endl;
    }
    std::cout << std::endl;
    std::cout << "----------------------------------" << std::endl;
}
