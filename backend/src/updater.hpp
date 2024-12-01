#pragma once

#include "websocket_server.hpp"
#include "robotic_crane.hpp"

class Updater {
public:
    Updater(frontend::WebsocketServer& server);

    void loop_send_positions();

private:
    std::string make_json(const robotics::crane& crane);

    frontend::WebsocketServer& server_;
};
    

