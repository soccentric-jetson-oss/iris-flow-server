// SPDX-License-Identifier: MIT
// Copyright (c) 2026 SoC Centric LLC
//
// iris_flow_service.hpp - gRPC service implementation for Iris Flow
//
/// @brief Declares the IrisFlowServiceImpl class that implements all
///        Iris Flow gRPC RPCs: stream control, ISP tuning, frame
///        capture, and health monitoring.

#pragma once

#include <iris_flow.grpc.pb.h>
#include <grpcpp/grpcpp.h>
#include <atomic>
#include <mutex>

namespace irisflow {

/// @brief Implements the IrisFlow gRPC service.
///
/// Handles camera stream lifecycle (start/stop), ISP parameter tuning,
/// frame retrieval, and health checks. In production, this delegates
/// to V4L2/GStreamer pipelines and the IMX708 sensor driver.
class IrisFlowServiceImpl final : public IrisFlow::Service {
public:
    IrisFlowServiceImpl();

    // ── Stream control ─────────────────────────────────────────────────
    grpc::Status GetStatus(grpc::ServerContext* context,
                           const StatusRequest* request,
                           StatusResponse* response) override;

    grpc::Status StartStream(grpc::ServerContext* context,
                             const StreamConfig* request,
                             StreamResponse* response) override;

    grpc::Status StopStream(grpc::ServerContext* context,
                            const StreamRequest* request,
                            StreamResponse* response) override;

    // ── Frame capture ───────────────────────────────────────────────────
    grpc::Status GetFrame(grpc::ServerContext* context,
                          const FrameRequest* request,
                          FrameResponse* response) override;

    grpc::Status StreamFrames(grpc::ServerContext* context,
                              const FrameRequest* request,
                              grpc::ServerWriter<FrameResponse>* writer) override;

    // ── ISP control ────────────────────────────────────────────────────
    grpc::Status SetIsp(grpc::ServerContext* context,
                        const IspConfig* request,
                        IspResponse* response) override;

    grpc::Status GetIsp(grpc::ServerContext* context,
                        const IspRequest* request,
                        IspConfig* response) override;

    // ── Health ─────────────────────────────────────────────────────────
    grpc::Status HealthCheck(grpc::ServerContext* context,
                             const HealthRequest* request,
                             HealthResponse* response) override;

private:
    std::mutex mutex_;
    std::atomic<bool> streaming_{false};
    IspConfig current_isp_;
    uint64_t start_time_us_;
    uint64_t frame_count_{0};

    static uint64_t NowUs();
};

} // namespace irisflow
