#include "websocket_server.hpp"

#include <json/json.h>
#include <iostream>
#include <thread>

namespace frontend {

WebsocketServer* WebsocketServer::instance_ = nullptr;

using server_t = WebsocketServer::server_t;

WebsocketServer::WebsocketServer(uint16_t port) {
    if (instance_ != nullptr) {
        throw std::runtime_error("Could not create WebsocketServer: an instance already exists.");
    }
    instance_ = this;

    // Set logging settings
    server_.set_access_channels(websocketpp::log::alevel::all);
    server_.clear_access_channels(websocketpp::log::alevel::frame_payload);
    server_.init_asio();

    server_.set_open_handler([this](websocketpp::connection_hdl hdl){
        clients_.push_back(hdl);
    });

    std::cout << "Listening on port " << port << "..." << std::endl;
    server_.set_reuse_addr(true);
    server_.listen(port);
    server_.start_accept();

    message_handler_ = [](const std::string&){
        throw std::runtime_error("Websocket error: A message was received but no message handler was set.");
    };

    std::signal(SIGINT, stop_instance);
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

void WebsocketServer::stop_instance(int signal) {
    if (instance_ != nullptr) {
        std::cout << "Received signal " << signal << ", shuting down server..." << std::endl;
        instance_->server_.stop_listening();
        instance_->server_.stop();
        exit(signal);
    }
}

}
