#include "updater.hpp"

#include <json/json.h>

Updater::Updater(frontend::WebsocketServer& server):
    server_(server),
    simulator_(simulation::Crane::default_crane(), simulation::default_crane_movement_config),
    planner_(simulation::default_crane_shape_config),
    crane_position_sensor_() {
        auto callback = std::bind(&Updater::handle_crane_target_msg, this, std::placeholders::_1);
        server_.set_message_handler(callback);
    }

void Updater::loop() {
    simulation::Crane crane(8'000, 90, -90, 20, 100);
    long duration = 0;

    while (true) {
        auto time_begin = std::chrono::system_clock::now().time_since_epoch().count();

        auto crane_pos = crane_position_sensor_.get_next_crane_position(static_cast<double>(duration) / SECOND_IN_NANOS);
        std::optional<simulation::Crane> new_crane_goal = std::nullopt;
        if (target_position_.has_value()) {
            planner_.set_crane_position(crane_pos);
            new_crane_goal = planner_.get_target_crane(*target_position_);
        }

        {  // begin locking
            auto lock = std::lock_guard<std::mutex>(mutex_);
            if (new_crane_goal.has_value()) {
                simulator_.set_goal_state(*new_crane_goal);
            }
            simulator_.simulate_next_step(std::max(duration, MESSAGE_RATE_NANOS));
        }  // end locking

        server_.send_all(make_json(simulator_.get_state(), crane_pos));

        auto time_end = std::chrono::system_clock::now().time_since_epoch().count();
        duration = time_end - time_begin;
        if (duration < MESSAGE_RATE_NANOS) {
            std::this_thread::sleep_for(std::chrono::nanoseconds(MESSAGE_RATE_NANOS - duration));
            duration = MESSAGE_RATE_NANOS;
        }
    }
}

std::string Updater::make_json(const simulation::Crane& crane, const simulation::CranePosition& crane_pos) {
    // Note: jsoncpp could save us the trouble but this is fine
    return "{\"liftHeight\": " + std::to_string(crane.lift_elevation_) +
            ",\"liftArmAngleDegree\": " + std::to_string(crane.swing_rotation_) +
            ",\"armForearmAngleDegree\": " + std::to_string(crane.elbow_rotation_) +
            ",\"forearmGripAngleDegree\": " + std::to_string(crane.wrist_rotation_) +
            ",\"gripperSpacing\": " + std::to_string(crane.grip_extension_) +
            ",\"craneX\": " + std::to_string(crane_pos.x_) +
            ",\"craneY\": " + std::to_string(crane_pos.y_) +
            ",\"craneZ\": " + std::to_string(crane_pos.z_) +
            ",\"craneRot\": " + std::to_string(crane_pos.rotation_) +
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

void Updater::handle_crane_target_msg(const std::string &msg) {
    std::cout << "Received message: " << msg << std::endl;

    try {
        auto values = parse_json(msg);
        if (values["type"].asString() == "coord") {
            const auto x = values["x"].asFloat();
            const auto y = values["y"].asFloat();
            const auto z = values["z"].asFloat();

            target_position_ = simulation::Position{x, y, z};
        } else if (values["type"].asString() == "crane") {
            const auto lift_height = values["lift_elevation"].asFloat();
            const auto swing_roation = values["swing_rotation"].asFloat();
            const auto elbow_rotation = values["elbow_rotation"].asFloat();
            const auto wrist_rotation = values["wrist_rotation"].asFloat();
            const auto grip_extension = values["grip_extension"].asFloat();

            auto goal_crane = simulation::Crane(lift_height, swing_roation, elbow_rotation, wrist_rotation, grip_extension);
            target_position_ = std::nullopt;
            goal_crane.normalize_angle();
            {
                auto lock = std::lock_guard<std::mutex>(mutex_);
                simulator_.set_goal_state(goal_crane);
            }

        } else if (values["type"].asString() == "4dof-speed") {
            const auto crane_speed_x = values["speed_x"].asFloat();
            const auto crane_speed_y = values["speed_y"].asFloat();
            const auto crane_speed_z = values["speed_z"].asFloat();
            const auto crane_rot_speed_y = values["speed_rot_y"].asFloat();

            const auto crane_speed = simulation::CraneSpeed{crane_speed_x, crane_speed_y, crane_speed_z, crane_rot_speed_y};            
            crane_position_sensor_.set_crane_speed(crane_speed);
        } else {
            throw std::runtime_error("Received message with unknown type: '" + values["type"].asString() + "'");
        }
    } catch (std::exception& e) {
        throw std::runtime_error("Received message which is malformed or missing elements: " + std::string(e.what()) + ". Message is '" + msg + "'");
    }
}
