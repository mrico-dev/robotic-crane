#include "frontend_interface.hpp"

#include <json/json.h>
#include <iostream>

namespace frontend {

using server_t = WebsocketConnection::server_t;


WebsocketConnection::WebsocketConnection(uint16_t port) {
    // Set logging settings
    server_.set_access_channels(websocketpp::log::alevel::all);
    server_.clear_access_channels(websocketpp::log::alevel::frame_payload);
    server_.init_asio();
    server_.listen(port);
    std::cout << "Listening on port " << port << "..." << std::endl;
    server_.start_accept();

    message_handler_ = [](const std::string&){
        throw std::runtime_error("Websocket error: A message was received but no message handler was set.");
    };
}

void WebsocketConnection::set_message_handler(std::function<void(const std::string&)> message_handler) {
    message_handler_ = message_handler;
    server_.set_message_handler([this](websocketpp::connection_hdl hdl, server_t::message_ptr msg) {
        message_handler_(msg->get_payload());
    });
}

void WebsocketConnection::run() {
    server_.run();
}

}
