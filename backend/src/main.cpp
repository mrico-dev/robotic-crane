#include "frontend_interface.hpp"

#include <iostream>

static constexpr uint16_t SERVER_PORT = 8080;

void print_message_handler(const std::string& msg) {
    std::cout << "Reveived message: " << std::endl << msg << std::endl;
}

int main(void) {
    auto connection = frontend::WebsocketConnection(SERVER_PORT);
    connection.set_message_handler(&print_message_handler);
    connection.run();

    std::cout << "Exiting..." << std::endl;
}
