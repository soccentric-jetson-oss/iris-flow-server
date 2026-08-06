// SPDX-License-Identifier: MIT
// Copyright (c) 2026 SoC Centric LLC
//
// iris_flow_service.cpp - gRPC service implementation for Iris Flow
//
/// @brief Implements all Iris Flow gRPC RPCs with simulated camera
///        and ISP behavior for development. In production, these
///        methods delegate to V4L2, GStreamer, and the IMX708 driver.

#include "service/iris_flow_service.hpp"
#include <iostream>
#include <thread>
#include <chrono>
#include <cstdlib>

namespace irisflow {

IrisFlowServiceImpl::IrisFlowServiceImpl()
    : start_time_us_(NowUs())
{
    current_isp_.set_brightness(128);
    current_isp_.set_contrast(128);
    current_isp_.set_saturation(128);
    current_isp_.set_sharpness(128);
}

uint64_t IrisFlowServiceImpl::NowUs()
{
    return static_cast<uint64_t>(
        std::chrono::duration_cast<std::chrono::microseconds>(
            std::chrono::steady_clock::now().time_since_epoch()
        ).count());
}

// ── Stream control ─────────────────────────────────────────────────────

grpc::Status IrisFlowServiceImpl::GetStatus(
    grpc::ServerContext*,
    const StatusRequest*,
    StatusResponse* response)
{
    response->set_state(streaming_.load() ? 1 : 0);
    response->set_fps(30);
    response->set_sensor("IMX708");
    return grpc::Status::OK;
}

grpc::Status IrisFlowServiceImpl::StartStream(
    grpc::ServerContext*,
    const StreamConfig* request,
    StreamResponse* response)
{
    std::lock_guard<std::mutex> lock(mutex_);

    if (request->width() > 0 && request->height() > 0) {
        std::cout << "Start stream: " << request->width()
                  << "x" << request->height()
                  << " @" << request->fps() << "fps\n";
    }

    streaming_.store(true);
    response->set_success(true);
    return grpc::Status::OK;
}

grpc::Status IrisFlowServiceImpl::StopStream(
    grpc::ServerContext*,
    const StreamRequest*,
    StreamResponse* response)
{
    streaming_.store(false);
    response->set_success(true);
    return grpc::Status::OK;
}

// ── Frame capture ───────────────────────────────────────────────────────

grpc::Status IrisFlowServiceImpl::GetFrame(
    grpc::ServerContext*,
    const FrameRequest*,
    FrameResponse* response)
{
    frame_count_++;
    response->set_frame_id(static_cast<uint32_t>(frame_count_));
    response->set_width(1920);
    response->set_height(1080);
    response->set_data("simulated_frame_data", 19);
    response->set_timestamp_ns(NowUs() * 1000);
    return grpc::Status::OK;
}

grpc::Status IrisFlowServiceImpl::StreamFrames(
    grpc::ServerContext* context,
    const FrameRequest* request,
    grpc::ServerWriter<FrameResponse>* writer)
{
    uint32_t count = (request->count() > 0) ? request->count() : 10;

    for (uint32_t i = 0; i < count; ++i) {
        if (context->IsCancelled())
            break;

        FrameResponse response;
        response.set_frame_id(++frame_count_);
        response.set_width(1920);
        response.set_height(1080);
        response.set_data("stream_frame_data", 17);
        response.set_timestamp_ns(NowUs() * 1000);

        if (!writer->Write(response))
            break;

        std::this_thread::sleep_for(std::chrono::milliseconds(33)); // ~30fps
    }

    return grpc::Status::OK;
}

// ── ISP control ────────────────────────────────────────────────────────

grpc::Status IrisFlowServiceImpl::SetIsp(
    grpc::ServerContext*,
    const IspConfig* request,
    IspResponse* response)
{
    std::lock_guard<std::mutex> lock(mutex_);
    current_isp_ = *request;
    response->set_success(true);
    return grpc::Status::OK;
}

grpc::Status IrisFlowServiceImpl::GetIsp(
    grpc::ServerContext*,
    const IspRequest*,
    IspConfig* response)
{
    std::lock_guard<std::mutex> lock(mutex_);
    *response = current_isp_;
    return grpc::Status::OK;
}

// ── Health ─────────────────────────────────────────────────────────────

grpc::Status IrisFlowServiceImpl::HealthCheck(
    grpc::ServerContext*,
    const HealthRequest*,
    HealthResponse* response)
{
    response->set_status("SERVING");
    response->set_version("0.1.0");
    return grpc::Status::OK;
}

} // namespace irisflow
