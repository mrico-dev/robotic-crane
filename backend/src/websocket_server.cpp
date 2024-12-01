#include "websocket_server.hpp"

#include <json/json.h>
#include <iostream>
#include <thread>

namespace frontend {

using server_t = WebsocketServer::server_t;

WebsocketServer::WebsocketServer(uint16_t port) {
    // Set logging settings
    server_.set_access_channels(websocketpp::log::alevel::all);
    server_.clear_access_channels(websocketpp::log::alevel::frame_payload);
    server_.init_asio();

    server_.set_open_handler([this](websocketpp::connection_hdl hdl){
        clients_.push_back(hdl);
    });
    /*server_.set_close_handler([this](websocketpp::connection_hdl hdl){
        clients_.erase(std::remove(clients_.begin(), clients_.end(), hdl), clients_.end());
    });*/

    std::cout << "Listening on port " << port << "..." << std::endl;
    server_.listen(port);
    server_.start_accept();


    message_handler_ = [](const std::string&){
        throw std::runtime_error("Websocket error: A message was received but no message handler was set.");
    };
}

void WebsocketServer::set_message_handler(std::function<void(const std::string&)> message_handler) {
    message_handler_ = message_handler;
    server_.set_message_handler([this](websocketpp::connection_hdl hdl, server_t::message_ptr msg) {
        message_handler_(msg->get_payload());
    });
}

void WebsocketServer::send_all(const std::string& data) {
    for (auto client: clients_) {
        try {
            server_.send(client, data.c_str(), data.size(), websocketpp::frame::opcode::text);
        } catch (std::exception& e) {
            std::cout << "Could not send msg to client: " << e.what() << std::endl;
        }
    }
}

void WebsocketServer::run() {
    auto thread = std::thread([this](){server_.run();});
    thread.detach();
}

}
