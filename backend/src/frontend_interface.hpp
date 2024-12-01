#pragma once

#include <boost/asio.hpp>
#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>

#include <string>
#include <cstdint>

namespace frontend {

class WebsocketConnection {
public:
    using server_t = websocketpp::server<websocketpp::config::asio>;

    WebsocketConnection(uint16_t port);

    void set_message_handler(std::function<void(const std::string&)> message_handler);

    void run();

private:
    server_t server_;
    std::function<void(const std::string&)> message_handler_;
};

}
 