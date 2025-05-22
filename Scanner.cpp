#include "Scanner.h"
#include <iostream>
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

#ifdef _WIN32
Scanner::WSAInitializer::WSAInitializer() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) == 0) {
        initialized_ = true;
    }
}

Scanner::WSAInitializer::~WSAInitializer() {
    if (initialized_) {
        WSACleanup();
    }
}
#endif

Scanner::SocketRAII::~SocketRAII() {
#ifdef _WIN32
    closesocket(sock_);
#else
    close(sock_);
#endif
}

Scanner::Scanner(const ScanConfig &config) : config(config), logger_(get_logger()) {}

void Scanner::scan_ports() {
    const int maximum_threads = config.threads;
    const std::string address = config.address;
    
    try {
        std::vector<std::thread> thread_pool; {
            std::unique_lock lock(mutex);
            for (int port : config.ports) {
                cv.wait(lock, [this, maximum_threads] {
                    return active_threads < maximum_threads;
                });

                active_threads++;
                logger_->debug("Scanning {}:{}", address, port);

                thread_pool.emplace_back([this, address, port] {
                    try {
                        const bool is_open = this->is_port_open(port);
                        std::lock_guard guard(mutex);

                        if (is_open) {
                            logger_->info("Port is open {}:{}", address, port);
                            open_ports.push_back(port);
                        } else {
                            logger_->debug("Port is closed {}:{}", address, port);
                        }
                    } catch (const std::exception& e) {
                        logger_->error("Error scanning port {}: {}", port, e.what());
                    }
                    
                    {
                        std::lock_guard guard(mutex);
                        active_threads--;
                    }
                    cv.notify_all();
                });
            }
        }

        for (auto& thread : thread_pool) {
            if (thread.joinable()) {
                thread.join();
            }
        }
    } catch (const std::exception& e) {
        logger_->error("Error in scan_ports: {}", e.what());
        throw;
    }
}

bool Scanner::is_port_open(const int port) {
    #ifdef _WIN32
    WSAInitializer wsa;
    #endif

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        logger_->error("Failed to create socket");
        return false;
    }

    SocketRAII sock_guard(sock);

    #ifdef _WIN32
    u_long mode = 1;
    ioctlsocket(sock, FIONBIO, &mode);
    #else
    fcntl(sock, F_SETFL, O_NONBLOCK);
    #endif

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    if (inet_pton(AF_INET, config.address.c_str(), &addr.sin_addr) <= 0) {
        logger_->error("Invalid address format");
        return false;
    }

    logger_->debug("Connecting to {}:{}", config.address, port);
    connect(sock, reinterpret_cast<sockaddr*>(&addr), sizeof(addr));

    fd_set writefds;
    FD_ZERO(&writefds);
    FD_SET(sock, &writefds);

    timeval tv{};
    tv.tv_sec = config.timeout;
    tv.tv_usec = 0;

    const int res = select(sock + 1, nullptr, &writefds, nullptr, &tv);
    if (res < 0) {
        logger_->error("Got select error {}", res);
        return false;
    }

    if (res > 0) {
        int so_error = 0;
        socklen_t len = sizeof(so_error);
        getsockopt(sock, SOL_SOCKET, SO_ERROR, reinterpret_cast<char*>(&so_error), &len);
        return so_error == 0;
    }

    return false;
}

void Scanner::print_summary() const {
    const auto open_ports_count = open_ports.size();
    std::cout << "\n----------------------------------\n"
              << "Found " << open_ports_count << " Open Ports!\n";
    
    for (const auto& port : open_ports) {
        std::cout << "port: " << port << '\n';
    }
    
    std::cout << "\n----------------------------------\n";
}