#include "websocket_server.hpp"
#include "updater.hpp"

#include <iostream>
#include <chrono>

static constexpr uint16_t SERVER_PORT = 8080;

int main(void) {
    auto server = frontend::WebsocketServer(SERVER_PORT);
    server.run();

    Updater updater(server);
    updater.loop();

    std::cout << "Main loop done." << std::endl;
}
