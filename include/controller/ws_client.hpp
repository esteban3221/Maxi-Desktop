// ixws_client.hpp
#pragma once

#include <ixwebsocket/IXWebSocket.h>
#include <ixwebsocket/IXNetSystem.h>
#include <nlohmann/json.hpp>
#include <string>
#include <functional>
#include <iostream>

class IXWSClient {
public:
    IXWSClient() {
        ix::initNetSystem();  // Inicializa red (una vez)
    }

    ~IXWSClient() {
        ws.disableAutomaticReconnection();
        ws.stop();
    }

    void connect(const std::string& url,
                 std::function<void()> on_open = nullptr,
                 std::function<void(const std::string&)> on_message = nullptr,
                 std::function<void(const std::string&)> on_error = nullptr,
                 std::function<void(int, const std::string&)> on_close = nullptr) {

        ws.setUrl(url);
        ws.setPingInterval(30);  // heartbeat cada 30s
        ws.setOnMessageCallback([this, on_open, on_message, on_error, on_close](
            const ix::WebSocketMessagePtr& msg) {

            switch (msg->type) {
                case ix::WebSocketMessageType::Open:
                    std::cout << "[IXWS] Conexión abierta!" << std::endl;
                    if (on_open) on_open();
                    break;

                case ix::WebSocketMessageType::Message:
                    if (on_message && msg->binary == false) {  // text
                        std::cout << "[IXWS] Mensaje: " << msg->str << std::endl;
                        on_message(msg->str);
                    }
                    break;

                case ix::WebSocketMessageType::Error:
                    std::cout << "[IXWS] Error: " << std::string(msg->errorInfo.reason) << std::endl;
                    if (on_error) on_error(std::string(msg->errorInfo.reason));
                    break;

                case ix::WebSocketMessageType::Close:
                    std::cout << "[IXWS] Cerrado: " << msg->closeInfo.reason << " (code " << msg->closeInfo.code << ")" << std::endl;
                    if (on_close) on_close(msg->closeInfo.code, msg->closeInfo.reason);
                    break;

                default:
                    break;
            }
        });

        ws.start();
    }

    void send(const std::string& payload) {
        ws.send(payload);
        std::cout << "[IXWS] Enviado: " << payload << std::endl;
    }

    void close() {
        ws.stop();
    }

private:
    ix::WebSocket ws;
};