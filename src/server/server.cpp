// SPDX-License-Identifier: MIT
// Copyright (c) 2026 SoC Centric LLC
//
// server.cpp - gRPC server wrapper implementation
//
/// @brief Implements server start, shutdown, and lifecycle management.

#include "server/server.hpp"
#include "service/iris_flow_service.hpp"
#include <iostream>

namespace irisflow {

Server::Server(const std::string& address)
    : address_(address)
{
}

grpc::Status Server::Start()
{
    IrisFlowServiceImpl service;

    grpc::ServerBuilder builder;
    builder.AddListeningPort(address_, grpc::InsecureServerCredentials());
    builder.RegisterService(&service);

    server_ = builder.BuildAndStart();
    if (!server_) {
        return grpc::Status(grpc::StatusCode::INTERNAL,
                            "Failed to start gRPC server");
    }

    running_ = true;
    std::cout << "Iris Flow Server listening on " << address_ << "\n";
    return grpc::Status::OK;
}

void Server::Shutdown()
{
    if (server_) {
        server_->Shutdown();
        running_ = false;
        std::cout << "Iris Flow Server stopped.\n";
    }
}

bool Server::IsRunning() const
{
    return running_;
}

} // namespace irisflow
