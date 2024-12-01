#include "updater.hpp"

Updater::Updater(frontend::WebsocketServer& server): server_(server) {}

void Updater::loop_send_positions() {
    robotics::crane crane(8, 90, -90, 20, .1);
    while (true) {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        crane.swing_rotation_ += 1;
        server_.send_all(make_json(crane));
    }
}

std::string Updater::make_json(const robotics::crane& crane) {
    // Note: some libraries could save us the trouble
    return "{\"liftHeight\": " + std::to_string(crane.lift_elevation_) +
            ",\"liftArmAngleDegree\": " + std::to_string(crane.swing_rotation_) +
            ",\"armForearmAngleDegree\": " + std::to_string(crane.elbow_rotation_) +
            ",\"forearmGripAngleDegree\": " + std::to_string(crane.wrist_rotation_) +
            ",\"gripperSpacing\": " + std::to_string(crane.grip_extenstion_) +
            "}";
}
