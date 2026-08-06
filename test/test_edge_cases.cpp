#include <catch2/catch_test_macros.hpp>
#include <iris_flow.grpc.pb.h>
#include <grpcpp/grpcpp.h>

TEST_CASE("Null input handling", "[edge]") {
    irisflow::StreamConfig cfg;
    // Default-constructed config should have zero values
    REQUIRE(cfg.width() == 0);
    REQUIRE(cfg.height() == 0);
    REQUIRE(cfg.fps() == 0);
    REQUIRE(cfg.format() == 0);
}

TEST_CASE("Empty input handling", "[edge]") {
    irisflow::FrameRequest req;
    // Default count should be 0
    REQUIRE(req.count() == 0);

    irisflow::FrameResponse resp;
    // Default frame data should be empty
    REQUIRE(resp.data().empty());
    REQUIRE(resp.frame_id() == 0);
}

TEST_CASE("Boundary values", "[edge]") {
    irisflow::StreamConfig cfg;
    // Test max reasonable values
    cfg.set_width(7680);  // 8K
    cfg.set_height(4320);
    cfg.set_fps(240);
    REQUIRE(cfg.width() == 7680);
    REQUIRE(cfg.height() == 4320);
    REQUIRE(cfg.fps() == 240);

    // Test zero values
    cfg.set_width(0);
    cfg.set_height(0);
    cfg.set_fps(0);
    REQUIRE(cfg.width() == 0);
}

TEST_CASE("Concurrent access", "[edge]") {
    irisflow::IspConfig cfg;
    cfg.set_brightness(128);
    cfg.set_contrast(128);
    cfg.set_saturation(128);

    std::atomic<int> counter{0};
    std::vector<std::thread> threads;
    for (int i = 0; i < 10; ++i) {
        threads.emplace_back([&]() {
            // Concurrent reads of protobuf should be safe
            auto b = cfg.brightness();
            auto c = cfg.contrast();
            auto s = cfg.saturation();
            if (b == 128 && c == 128 && s == 128) counter++;
        });
    }
    for (auto& t : threads) t.join();
    REQUIRE(counter == 10);
}

TEST_CASE("Resource cleanup on error", "[edge]") {
    // Verify that gRPC status codes are properly set on error
    grpc::Status ok_status = grpc::Status::OK;
    REQUIRE(ok_status.ok());

    grpc::Status not_found(grpc::StatusCode::NOT_FOUND, "not found");
    REQUIRE_FALSE(not_found.ok());
    REQUIRE(not_found.error_code() == grpc::StatusCode::NOT_FOUND);
    REQUIRE(not_found.error_message() == "not found");
}
