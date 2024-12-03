#pragma once

#include "websocket_server.hpp"
#include "datamodels.hpp"
#include "crane_simulator.hpp"
#include "crane_planner.hpp"

class Updater {
    static constexpr int64_t MESSAGE_RATE_NANOS = 50'000'000;

public:
    Updater(frontend::WebsocketServer& server);

    void loop_send_positions();

private:
    std::string make_json(const simulation::Crane& crane);

    void handle_crane_target_msg(const std::string& msg);

    std::mutex mutex_;
    frontend::WebsocketServer& server_;
    simulation::CraneSimulator simulator_;
    simulation::CranePlanner planner_;
};
    

