// ws_client.hpp  (o ixws_client.hpp)
#pragma once
#include <ixwebsocket/IXWebSocket.h>
#include <ixwebsocket/IXNetSystem.h>
#include <string>
#include <functional>
#include <iostream>

class IXWSClient {
public:
    IXWSClient() {
        static std::once_flag init_flag;
        std::call_once(init_flag, []() {
            ix::initNetSystem();
            std::cout << "[IXWS] Red inicializada (Unica)\n";
        });
    }

    ~IXWSClient() {
        ws.disableAutomaticReconnection();
        ws.stop();
    }

    void connect(const std::string& url,
                 std::function<void()> on_open = nullptr,
                 std::function<void(const std::string&)> on_message = nullptr,
                 std::function<void(const std::string&)> on_error = nullptr,
                 std::function<void(int, const std::string&)> on_close = nullptr,
                 bool debug = false) {

        ws.setUrl(url);
        ws.setPingInterval(30);

        ws.setOnMessageCallback([on_open, on_message, on_error, on_close, debug, url](
            const ix::WebSocketMessagePtr& msg) {

            switch (msg->type) {
                case ix::WebSocketMessageType::Open:
                    std::cout << "[WS] Conexión abierta → " << url << "\n";
                    if (on_open) on_open();
                    break;

                case ix::WebSocketMessageType::Message:
                    if (on_message && !msg->binary) {
                        if (debug) std::cout << "[WS] Mensaje: " << msg->str << "\n";
                        on_message(msg->str);
                    }
                    break;

                case ix::WebSocketMessageType::Error:
                    std::cout << "[WS] Error: " << msg->errorInfo.reason << "\n";
                    if (on_error) on_error(msg->errorInfo.reason);
                    break;

                case ix::WebSocketMessageType::Close:
                    std::cout << "[WS] Cerrado (" << msg->closeInfo.code << "): "
                              << msg->closeInfo.reason << "\n";
                    if (on_close) on_close(msg->closeInfo.code, msg->closeInfo.reason);
                    break;
            }
        });

        ws.start();
    }

    // Versión simple: sin url como parámetro
    void send(const std::string& payload) {
        ws.send(payload);
        std::cout << "[WS] Enviado: " << payload << "\n";
    }

    void close() {
        ws.stop();
        std::cout << "[WS] Conexión cerrada manualmente\n";
    }

private:
    ix::WebSocket ws;
};