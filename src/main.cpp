#include <csignal>
#include <atomic>
static std::atomic<bool> g_running{true};
static void signal_handler(int) { g_running.store(false); }
#include <iostream>
#include <grpcpp/grpcpp.h>
#include <iris_flow.grpc.pb.h>

class IrisFlowServiceImpl final : public irisflow::IrisFlow::Service {
    grpc::Status GetStatus(grpc::ServerContext*, const irisflow::StatusRequest*, irisflow::StatusResponse* resp) override {
        resp->set_state(1); resp->set_fps(30); resp->set_sensor("IMX708");
        return grpc::Status::OK;
    }
    grpc::Status StartStream(grpc::ServerContext*, const irisflow::StreamConfig* req, irisflow::StreamResponse* resp) override {
        std::cout << "Start stream: " << req->width() << "x" << req->height() << "\n";
        resp->set_success(true);
        return grpc::Status::OK;
    }
    grpc::Status StopStream(grpc::ServerContext*, const irisflow::StreamRequest*, irisflow::StreamResponse* resp) override {
        resp->set_success(true);
        return grpc::Status::OK;
    }
    grpc::Status GetFrame(grpc::ServerContext*, const irisflow::FrameRequest*, irisflow::FrameResponse* resp) override {
        resp->set_frame_id(1); resp->set_width(1920); resp->set_height(1080);
        resp->set_data("simulated_frame_data", static_cast<size_t>(16));
        resp->set_timestamp_ns(1000000);
        return grpc::Status::OK;
    }
    grpc::Status SetIsp(grpc::ServerContext*, const irisflow::IspConfig* /*req*/, irisflow::IspResponse* resp) override {
        resp->set_success(true);
        return grpc::Status::OK;
    }
    grpc::Status GetIsp(grpc::ServerContext*, const irisflow::IspRequest*, irisflow::IspConfig* resp) override {
        resp->set_brightness(128); resp->set_contrast(128); resp->set_saturation(128);
        return grpc::Status::OK;
    }
    grpc::Status HealthCheck(grpc::ServerContext*, const irisflow::HealthRequest*, irisflow::HealthResponse* resp) override {
        resp->set_status("SERVING"); resp->set_version("0.1.0");
        return grpc::Status::OK;
    }
};

int main() {
    std::string addr = "0.0.0.0:50052";
    IrisFlowServiceImpl service;
    grpc::ServerBuilder builder;
    builder.AddListeningPort(addr, grpc::InsecureServerCredentials());
    builder.RegisterService(&service);
    auto server = builder.BuildAndStart();
    std::cout << "Iris Flow Server listening on " << addr << "\n";
    while (g_running.load()) { std::this_thread::sleep_for(std::chrono::milliseconds(100)); }
    std::cout << "Shutting down...\n";
    server->Shutdown();
    std::cout << "Done.\n";
    return 0;
}

/* FFmpeg integration for capture->encode->process->store->stream pipeline */
extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/imgutils.h>
}

static void init_ffmpeg_pipeline(void) {
    avformat_network_init();
    std::cout << "FFmpeg pipeline initialized" << std::endl;
}

/* Display/graphics handoff */
static void handoff_to_display(const uint8_t* frame_data, int width, int height) {
    /* In production, this would use DRM/KMS or Wayland to display the frame */
    std::cout << "Display handoff: " << width << "x" << height << std::endl;
}
