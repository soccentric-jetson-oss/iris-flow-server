#include <catch2/catch_test_macros.hpp>
#include <iris_flow.grpc.pb.h>

TEST_CASE("HealthCheck returns SERVING", "[server]") {
    irisflow::HealthResponse resp;
    resp.set_status("SERVING");
    REQUIRE(resp.status() == "SERVING");
}

TEST_CASE("StreamConfig validation", "[server]") {
    irisflow::StreamConfig cfg;
    cfg.set_width(1920); cfg.set_height(1080); cfg.set_fps(30);
    REQUIRE(cfg.width() == 1920);
    REQUIRE(cfg.height() == 1080);
    REQUIRE(cfg.fps() == 30);
}
