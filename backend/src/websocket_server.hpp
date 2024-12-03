#pragma once

#include <boost/asio.hpp>
#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>

#include <set>
#include <string>
#include <cstdint>

namespace frontend {

class WebsocketServer {
public:
    using server_t = websocketpp::server<websocketpp::config::asio>;

    WebsocketServer(uint16_t port);

    void set_message_handler(std::function<void(const std::string&)>&& message_handler);

    void send_all(const std::string& data);

    void run();

private:
    static void stop_instance(int signal);

    server_t server_;
    std::function<void(const std::string&)> message_handler_;
    std::set<websocketpp::connection_hdl, std::owner_less<>> clients_;
    std::thread server_thread_;

    static WebsocketServer* instance_;
};

}
 