// SPDX-License-Identifier: MIT
// Copyright (c) 2026 SoC Centric LLC
//
// server.hpp - gRPC server wrapper for Iris Flow
//
/// @brief Manages the gRPC server lifecycle: start, shutdown, and
///        signal handling. Thin wrapper around grpc::Server.

#pragma once

#include <string>
#include <memory>
#include <grpcpp/server.h>
#include <grpcpp/server_builder.h>

namespace irisflow {

/// @brief Wraps a gRPC server with Iris Flow service implementation.
class Server {
public:
    /// @brief Construct a server bound to the given address.
    /// @param address Host:port string (e.g., "127.0.0.1:50052").
    explicit Server(const std::string& address);

    /// @brief Start the gRPC server.
    /// @return OK on success, error status otherwise.
    grpc::Status Start();

    /// @brief Gracefully shut down the server.
    void Shutdown();

    /// @brief Check if the server is currently running.
    bool IsRunning() const;

private:
    std::string address_;
    std::unique_ptr<grpc::Server> server_;
    bool running_{false};
};

} // namespace irisflow
