#pragma once

#include "websocket_server.hpp"
#include "datamodels.hpp"
#include "crane_simulator.hpp"
#include "crane_planner.hpp"
#include "DOF4_sensor.hpp"

#include <optional>

class Updater {
    static constexpr int64_t MESSAGE_RATE_NANOS = 50'000'000;
    static constexpr int64_t SECOND_IN_NANOS = 1'000'000'000;

public:
    Updater(frontend::WebsocketServer& server);

    void loop();

private:
    static std::string make_json(const simulation::Crane& crane, const simulation::CranePosition& crane_pos);

    void handle_crane_target_msg(const std::string& msg);

    std::optional<simulation::Position> target_position_;

    std::mutex mutex_;
    frontend::WebsocketServer& server_;
    simulation::CraneSimulator simulator_;
    simulation::CranePlanner planner_;
    simulation::DOF4Sensor crane_position_sensor_;
};
    

