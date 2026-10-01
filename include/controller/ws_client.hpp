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
            g_info ("[IXWS] Red inicializada (Unica)");
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
                    g_message("[WS] Conexión abierta → %s", url.c_str());
                    if (on_open) on_open();
                    break;

                case ix::WebSocketMessageType::Message:
                    if (on_message && !msg->binary) {
                        if (debug) g_message("[WS] Mensaje: %s", msg->str.c_str());
                        on_message(msg->str);
                    }
                    break;

                case ix::WebSocketMessageType::Error:
                    g_critical("[WS] Error: %s", msg->errorInfo.reason.c_str());
                    if (on_error) on_error(msg->errorInfo.reason);
                    break;

                case ix::WebSocketMessageType::Close:
                    g_message("[WS] Cerrado (%d): %s", msg->closeInfo.code, msg->closeInfo.reason.c_str());
                    if (on_close) on_close(msg->closeInfo.code, msg->closeInfo.reason);
                    break;
            }
        });

        ws.start();
    }

    // Versión simple: sin url como parámetro
    void send(const std::string& payload) {
        ws.send(payload);
        g_message("[WS] Enviado: %s", payload.c_str());
    }

    void close() {
        ws.stop();
        g_message("[WS] Conexión cerrada manualmente");
    }

private:
    ix::WebSocket ws;
};