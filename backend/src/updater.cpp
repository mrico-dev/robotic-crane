#include "updater.hpp"

static constexpr int64_t MESSAGE_RATE_NANOS = 10'000'000; // 10 millis

Updater::Updater(frontend::WebsocketServer& server): server_(server) {}

void Updater::loop_send_positions() {
    robotics::crane crane(8, 90, -90, 20, .1);
    while (true) {
        auto time_begin = std::chrono::system_clock::now().time_since_epoch().count();

        crane.swing_rotation_ += 1;
        server_.send_all(make_json(crane));

        auto time_end = std::chrono::system_clock::now().time_since_epoch().count();
        auto duration = time_end - time_begin;
        if (duration < MESSAGE_RATE_NANOS) {
            std::this_thread::sleep_for(std::chrono::nanoseconds(MESSAGE_RATE_NANOS - duration));
        }
    }
}

std::string Updater::make_json(const robotics::crane& crane) {
    // Note: some libraries could save us the trouble but this is fine
    return "{\"liftHeight\": " + std::to_string(crane.lift_elevation_) +
            ",\"liftArmAngleDegree\": " + std::to_string(crane.swing_rotation_) +
            ",\"armForearmAngleDegree\": " + std::to_string(crane.elbow_rotation_) +
            ",\"forearmGripAngleDegree\": " + std::to_string(crane.wrist_rotation_) +
            ",\"gripperSpacing\": " + std::to_string(crane.grip_extenstion_) +
            "}";
}
