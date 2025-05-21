#include "Scanner.h"
#include <iostream>
#include <mutex>
#include <thread>

Scanner::Scanner(const ScanConfig &config) {
    this->config = config;
}

void Scanner::scanPorts() {
    std::vector<std::thread> thread_pool;
    const int maximum_threads = this->config.threads;
    const std::string address = this->config.address;
    const bool verbose = this->config.verbose;
    const int timeout = this->config.timeout;

    for (size_t i = 0; i < this->config.ports.size(); ++i) {
        const int port = this->config.ports[i];
        std::unique_lock lock(this->mutex);
        cv.wait(lock, [this, maximum_threads] { return this->active_threads < maximum_threads; });
        this->active_threads++;
        if (verbose) {
            std::cout << "Scanning " << address << ":" << port << std::endl;
        }
        thread_pool.emplace_back([this, address, port, timeout, verbose]() {
            this->isPortOpen(address, port, timeout); {
                std::lock_guard lock(this->mutex);
                this->active_threads--;
                if (verbose) {
                    std::cout << "Finished scanning " << address << ":" << port << std::endl;
                }
            }
            cv.notify_all();
        });
        for (auto &t: thread_pool) {
            if (t.joinable())
                t.join();
        }
    }
}

void Scanner::isPortOpen(const std::string &address, int port, int timeout) {

}
