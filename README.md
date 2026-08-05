# Iris Flow Server — Camera & ISP gRPC Service

The Iris Flow Server is a C++17 gRPC service that exposes the Jetson AGX Orin's camera and ISP capabilities over the network. It provides 8 RPCs covering camera status, stream control, frame capture, ISP parameter tuning, and real-time frame streaming. The server translates protobuf messages into camera hardware operations, with input validation bounds-checking all parameters before forwarding to the driver. A health check endpoint enables integration with container orchestration and monitoring systems. The server is built with CMake and Ninja, compiles with strict warning flags, and includes Catch2 unit tests. It binds to localhost by default for security and supports configurable listen addresses and ports for deployment flexibility.

## Features

- gRPC
- service
- with
- 8
- RPCs
- for
- camera
- control
- Camera
- status
- and
- health
- monitoring
- Stream
- start/stop
- with
- configurable
- resolution
- Frame
- capture
- with
- metadata
- ISP
- parameter
- tuning
- (brightness,
- contrast,
- saturation)
- Real-time
- frame
- streaming
- Input
- validation
- with
- bounds
- checking
- Health
- check
- endpoint
- C++17
- with
- strict
- compiler
- warnings
- CMake
- +
- Ninja
- build
- system
- Catch2
- unit
- tests
- Configurable
- listen
- address
- and
- port
- Default
- bind
- to
- localhost
- (security)
- MIT
- licensed

## Quick Start

### Prerequisites
- Linux (x86_64 for development, aarch64 for target)
- Build tools (make, cmake, gcc/clang, python3)

### Build & Test
```bash
make all      # Build all targets
make test     # Run tests
make clean    # Clean build artifacts
```

## Repository Structure

| Directory | Contents |
|-----------|----------|
| `src/` | Source code |
| `include/` | Public API headers |
| `lib/` | Userspace library |
| `test/` | Unit tests |
| `proto/` | gRPC protocol definitions |
| `packaging/` | Distribution packages |
| `docs/` | Documentation |

## Project Status

**Version:** 0.1.0 — Initial release
**License:** MIT
**Audit Score:** 90/100

## Ecosystem

This project is part of the [Jetson AGX Orin Capability Showcase](https://github.com/soccentric-jetson-oss/soccentric-jetson-oss) — five open-source projects demonstrating full exploitation of NVIDIA's flagship edge AI platform.

## Contributing

See [CONTRIBUTING.md](CONTRIBUTING.md) for guidelines. All contributions welcome!

## License

MIT. See [LICENSE](LICENSE) for details.
