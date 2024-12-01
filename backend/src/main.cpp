#include "websocket_server.hpp"
#include "updater.hpp"

#include <iostream>
#include <chrono>

static constexpr uint16_t SERVER_PORT = 8080;

void print_message_handler(const std::string& msg) {
    std::cout << "Reveived message: " << std::endl << msg << std::endl;
}


int main(void) {
    auto server = frontend::WebsocketServer(SERVER_PORT);
    server.set_message_handler(&print_message_handler);
    server.run();

    Updater updater(server);
    updater.loop_send_positions();

    std::cout << "Main loop done." << std::endl;
}
