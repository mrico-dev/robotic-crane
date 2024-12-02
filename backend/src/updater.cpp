#include "updater.hpp"

#include <json/json.h>

Updater::Updater(frontend::WebsocketServer& server):
    server_(server),
    simulator_(simulation::Crane::default_crane(), simulation::default_crane_movement_config),
    planner_(simulation::default_crane_shape_config) {}

void Updater::loop_send_positions() {
    simulation::Crane crane(8'000, 90, -90, 20, 100);
    long duration = 0;

    while (true) {
        auto time_begin = std::chrono::system_clock::now().time_since_epoch().count();

        simulator_.simulate_next_step(std::max(duration, MESSAGE_RATE_NANOS));
        server_.send_all(make_json(simulator_.get_state()));

        auto time_end = std::chrono::system_clock::now().time_since_epoch().count();
        duration = time_end - time_begin;
        if (duration < MESSAGE_RATE_NANOS) {
            std::this_thread::sleep_for(std::chrono::nanoseconds(MESSAGE_RATE_NANOS - duration));
        }
    }
}

std::string Updater::make_json(const simulation::Crane& crane) {
    // Note: jsoncpp could save us the trouble but this is fine
    return "{\"liftHeight\": " + std::to_string(crane.lift_elevation_) +
            ",\"liftArmAngleDegree\": " + std::to_string(crane.swing_rotation_) +
            ",\"armForearmAngleDegree\": " + std::to_string(crane.elbow_rotation_) +
            ",\"forearmGripAngleDegree\": " + std::to_string(crane.wrist_rotation_) +
            ",\"gripperSpacing\": " + std::to_string(crane.grip_extension_) +
            "}";
}

Json::Value parse_json(const std::string &msg) {
    Json::Value root;
    std::string errs;

    std::istringstream jsonStream(msg);
    if (!Json::parseFromStream(Json::CharReaderBuilder{}, jsonStream, &root, &errs)) {
        throw std::runtime_error("Could not parse Json: " + errs + "\nThe JSON was: " + msg);
    }

    return root;
}

void Updater::set_crane_target(const std::string &msg) {
    auto values = parse_json(msg);

    const auto x = values["x"].asFloat();
    const auto y = values["y"].asFloat();
    const auto z = values["z"].asFloat();

    const auto position = simulation::EulerPosition{x, y, z};
    const auto goal_crane = planner_.get_target_crane(position);

    simulator_.set_goal_state(goal_crane);
}
