// ws_client.hpp
#pragma once

#include <websocketpp/config/asio_no_tls_client.hpp>
#include <websocketpp/client.hpp>
#include <websocketpp/common/thread.hpp>
#include <nlohmann/json.hpp>
#include <iostream>
#include <string>
#include <functional>

typedef websocketpp::client<websocketpp::config::asio_client> client;

class WSClient {
public:
    WSClient() {
        m_client.init_asio();
        m_client.clear_access_channels(websocketpp::log::alevel::all);  // Silencia logs si quieres
        // m_client.set_access_channels(websocketpp::log::alevel::connect | websocketpp::log::alevel::disconnect | websocketpp::log::alevel::control);
    }

    ~WSClient() {
        close();
    }

    void connect(const std::string& uri,
                 std::function<void()> on_open_callback = nullptr,
                 std::function<void(const std::string&)> on_message_callback = nullptr,
                 std::function<void(websocketpp::close::status::value, const std::string&)> on_close_callback = nullptr) {

        on_open_cb = on_open_callback;
        on_message_cb = on_message_callback;
        on_close_cb = on_close_callback;

        websocketpp::lib::error_code ec;
        client::connection_ptr con = m_client.get_connection(uri, ec);
        if (ec) {
            std::cerr << "Error al obtener conexión: " << ec.message() << std::endl;
            return;
        }

        m_hdl = con->get_handle();

        m_client.set_open_handler([this](websocketpp::connection_hdl) {
            if (on_open_cb) on_open_cb();
        });

        m_client.set_message_handler([this](websocketpp::connection_hdl, client::message_ptr msg) {
            if (on_message_cb && msg->get_opcode() == websocketpp::frame::opcode::text) {
                on_message_cb(msg->get_payload());
            }
        });

        m_client.set_close_handler([this](websocketpp::connection_hdl) {
            if (on_close_cb) on_close_cb(websocketpp::close::status::normal, "Closed");
        });

        m_client.connect(con);

        // Thread para correr el io_context (asíncrono)
        m_thread = websocketpp::lib::make_shared<websocketpp::lib::thread>(&client::run, &m_client);
    }

    void send(const std::string& payload) {
        websocketpp::lib::error_code ec;
        m_client.send(m_hdl, payload, websocketpp::frame::opcode::text, ec);
        if (ec) {
            std::cerr << "Error al enviar: " << ec.message() << std::endl;
        }
    }

    void close() {
        websocketpp::lib::error_code ec;
        m_client.close(m_hdl, websocketpp::close::status::normal, "", ec);
        if (ec) std::cerr << "Error al cerrar: " << ec.message() << std::endl;

        if (m_thread && m_thread->joinable()) {
            m_thread->join();
        }
    }

private:
    client m_client;
    websocketpp::connection_hdl m_hdl;
    websocketpp::lib::shared_ptr<websocketpp::lib::thread> m_thread;

    std::function<void()> on_open_cb;
    std::function<void(const std::string&)> on_message_cb;
    std::function<void(websocketpp::close::status::value, const std::string&)> on_close_cb;
};
