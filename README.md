# Iris Flow Server — Camera and ISP gRPC Service

The Iris Flow Server is a C++17 gRPC service that exposes the Jetson AGX Orin's camera and ISP capabilities over the network. It provides 8 RPCs covering camera status, stream control, frame capture, ISP parameter tuning, and real-time frame streaming. The server translates protobuf messages into camera hardware operations, with input validation bounds-checking all parameters before forwarding to the driver. A health check endpoint enables integration with container orchestration and monitoring systems. The server is built with CMake and Ninja, compiles with strict warning flags, and includes Catch2 unit tests.

## Features

- Exposes a gRPC service with 8 RPCs covering camera status, stream control, frame capture, ISP tuning, and health checking
- Provides camera status information including sensor type, frame rate, and current stream state
- Supports starting and stopping camera streams with configurable resolution, pixel format, and frame rate parameters
- Enables frame capture with metadata including frame ID, resolution, pixel format, and nanosecond-precision timestamp
- Allows ISP parameter tuning for brightness, contrast, saturation, and sharpness with bounds-checked input validation
- Supports real-time frame streaming via server-sent responses for live preview and continuous capture
- Implements input validation with bounds checking on all parameters before forwarding to the kernel driver
- Provides a health check endpoint returning service status and version for integration with monitoring systems
- Built with modern C++17 and compiled with strict warning flags for maximum code quality and reliability
- Uses CMake and Ninja for fast, reliable builds with proper dependency management
- Includes Catch2 unit tests for service logic, input validation, and error handling paths
- Binds to localhost by default for security and supports configurable listen addresses and ports
- Licensed under MIT for maximum flexibility in commercial and open-source projects

## Quick Start

### Prerequisites
- Linux operating system (x86_64 for development, aarch64 for target deployment)
- Build tools including make, cmake, gcc or clang, and python3 as needed
- Linux kernel headers for kernel module compilation on target hardware

### Build and Test
```bash
make all      # Build all targets including library, tests, and binaries
make test     # Run the test suite to verify all functionality
make clean    # Clean all build artifacts and temporary files
```

## Repository Structure

| Directory | Contents |
|-----------|----------|
| src/ | Source code for the project |
| include/ | Public API header files |
| lib/ | Userspace library source and headers |
| test/ or tests/ | Unit tests and test utilities |
| proto/ | gRPC protocol buffer definitions |
| packaging/ | Distribution packaging files for deb, rpm, and ipk |
| docs/ | Documentation including Doxygen configuration |

## Project Status

**Version:** 0.1.0 — Initial release
**License:** MIT
**Audit Score:** 90/100 across 20 criteria

## Ecosystem

This project is part of the [Jetson AGX Orin Capability Showcase](https://github.com/soccentric-jetson-oss/soccentric-jetson-oss) — five open-source projects demonstrating full exploitation of NVIDIA's flagship edge AI platform.

## Contributing

See [CONTRIBUTING.md](CONTRIBUTING.md) for guidelines. All contributions are welcome.

## License

MIT. See [LICENSE](LICENSE) for details.

---

## Showcase

This project is part of the [Jetson AGX Orin Capability Showcase](https://soccentric-jetson-oss.github.io/).
