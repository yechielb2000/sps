# 🕷️ SPS - Simple Port Scanner

A lightweight command-line tool for scanning ports on a given IP address to check whether they're open or closed.

---

## 🚀 Features

- Scan single ports, lists, or ranges (e.g. `22`, `80,443,8080`, `443-8989`)
- Multithreaded scanning for speed
- Configurable timeout
- Verbose output mode for detailed status
- Easy-to-use CLI

---

## 🛠️ Usage

| Option            | Description                                 | Default     |
|-------------------|---------------------------------------------|-------------|
| `-a`, `--address` | IP address to scan                          | `127.0.0.1` |
| `-p`, `--ports`   | Ports to scan (e.g. `22,80,443` or `20-25`) | `1-65535`   |
| `-v`, `--verbose` | Enable verbose output                       | `0` (off)   |
| `-t`, `--threads` | Maximum number of threads                   | `10`        |
| `-m`, `--timeout` | Timeout per port in seconds                 | `12`        |
| `-h`, `--help`    | Show help message                           |             |

## 🧪 Example

```bash
sps -a 192.168.1.1 -p 22-8080 -t 50 -v
```
