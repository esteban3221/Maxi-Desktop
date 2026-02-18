#include "controller/venta.hpp"

Venta::Venta(bool is_view_ingreso) : is_view_ingreso(is_view_ingreso)
{
    v_base_nip->v_btn_nip_enter->signal_clicked().connect(sigc::mem_fun(*this, &Venta::on_btn_enter_clicked));
    v_base_nip->v_ety_spin->set_text("");
    this->signal_map().connect(sigc::mem_fun(*this, &Venta::on_map_show));
    v_ety_concepto.signal_activate().connect([this](){ v_base_nip->v_ety_spin->grab_focus(); });
    v_base_nip->v_ety_spin->signal_activate().connect([this](){ v_base_nip->v_btn_nip_enter->grab_focus(); });
    v_box_columns->v_btn_cancelar->signal_clicked().connect(sigc::mem_fun(*this, &Venta::on_btn_cancelar_clicked));
}

Venta::~Venta()
{
}

void Venta::on_btn_cancelar_clicked()
{
    v_dialog.reset(new Gtk::MessageDialog(*Global::Widget::v_main_window, std::string("Cancelar ") + (is_view_ingreso ? "Ingreso" : "Venta"), false, Gtk::MessageType::QUESTION, Gtk::ButtonsType::OK_CANCEL));
    v_dialog->set_secondary_text("¿Está seguro que desea cancelar la operación?");
    v_dialog->signal_response().connect([this](int response_id)
    {
        if (response_id == Gtk::ResponseType::OK)
        {
            ws.send(nlohmann::json{{"action", "detener"}}.dump());
            Global::Widget::reveal_toast(Glib::ustring::compose("<span weight=\"bold\"> %1 Cancelada </span>", is_view_ingreso ? "Ingreso" : "Venta"), Gtk::MessageType::OTHER);
                
            set_sensitive(true); 
        }
        v_dialog->close();
    });
    v_dialog->set_hide_on_close();
    v_dialog->set_visible();
}

void Venta::on_btn_enter_clicked()
{
    if (v_base_nip->v_ety_spin->get_value_as_int() < 1)
    {
        v_dialog.reset(new Gtk::MessageDialog(*Global::Widget::v_main_window, "Info", false, Gtk::MessageType::INFO, Gtk::ButtonsType::NONE));
        v_dialog->set_secondary_text("El monto a vender debe ser mayor a 0");
        v_dialog->set_hide_on_close();
        v_dialog->set_visible();
        return;
    }

    // Deshabilita el botón para evitar múltiples clics
    v_revealer_columns.set_reveal_child(true);
    v_base_nip->set_sensitive(false);
    v_ety_concepto.set_sensitive(false);
    v_base_nip->v_ety_spin->update();

    ws.connect(Global::System::WS +"/ws/venta",
    sigc::mem_fun(*this, &Venta::enviar_datos_venta),
    sigc::mem_fun(*this, &Venta::manejar_respuesta_servidor),
    [this](const std::string& err) {
        Global::Widget::reveal_toast(Glib::ustring::compose("Error de conexión: %1", err), (Gtk::MessageType)3, 5000);
    },
    [this](int code, const std::string& reason) {
        g_info("Conexión cerrada: %s (código %d)", reason.c_str(), code);
    });

    auto value = v_base_nip->v_ety_spin->get_value_as_int();
    auto json = nlohmann::json
    {
        {"value", value},
        {"concepto", v_ety_concepto.get_text()},
        {"is_view_ingreso", is_view_ingreso}
    };
    auto future = cpr::PostAsync(cpr::Url{Global::System::URL + "accion/inicia_venta"}, Global::Utility::header, cpr::Body{json.dump()});
    Global::Utility::consume_and_do(future, [this](const cpr::Response &response)
    {
        if (response.status_code == 200) 
        {
            auto j = nlohmann::json::parse(response.text);
            auto log = std::make_unique<Log>();
            auto m_log = log->get_log(j["ticket"]);
            auto ticket = m_log->get_item(0);
            auto faltante = j["Cambio_faltante"].get<int>();

            Global::Widget::reveal_toast(Glib::ustring::compose("<span weight=\"bold\"> %5 </span>\n\n"
                                                                "Total: \t\t$%1\n"
                                                                "Cambio: \t$%2\n"
                                                                "Ingreso: \t$%3\n"
                                                                "Faltante: \t$%4\n", 
                                                                ticket->m_total, 
                                                                ticket->m_cambio, 
                                                                ticket->m_ingreso, 
                                                                faltante,
                                                                is_view_ingreso ? "Ingreso" : "Venta"), Gtk::MessageType::OTHER);
            Global::System::imprime_ticket(ticket, faltante);
            if (faltante > 0) 
            {
                v_dialog.reset(new Gtk::MessageDialog(*Global::Widget::v_main_window,"Cambio Faltante",false,Gtk::MessageType::INFO, Gtk::ButtonsType::NONE));
                v_dialog->set_secondary_text(Glib::ustring::format("Se requiere un cambio de " , faltante));
                v_dialog->set_visible();
            }
        } else 
        {
            v_dialog.reset(new Gtk::MessageDialog(*Global::Widget::v_main_window, "Error"));
            v_dialog->set_secondary_text(response.text);
            v_dialog->set_visible();
        }
        Global::Widget::m_refActionGroup->lookup_action("cerrarsesion")->activate();
        set_sensitive(true); 
    });
}

void Venta::on_map_show()
{
    v_base_nip->v_ety_spin->set_text("");
    v_revealer_columns.set_reveal_child(false);
    v_ety_concepto.set_text("");
    v_ety_concepto.grab_focus();
    v_base_nip->set_sensitive(true);
    v_ety_concepto.set_sensitive(true);
}


// WBSocket methods

void Venta::enviar_datos_venta()
{
    // auto json = nlohmann::json
    // {
    //     {"action", "consulta"}
    // };
    // ws.send(json.dump());
}

void Venta::manejar_respuesta_servidor(const std::string& respuesta)
{
    try 
    {
        auto json = nlohmann::json::parse(respuesta);
        std::string status = json["status"].get<std::string>();

        if (status == "Proceso terminado") {
            Glib::signal_idle().connect_once([this]() {
                ws.close();
                g_info("WebSocket cerrado desde hilo principal");
            });
            return; 
        }

        Glib::signal_idle().connect_once([this, json]() {
            if (json.contains("total")) {
                v_box_columns->v_ety_columns[0]->set_text(Glib::ustring::compose("$ %1", json["total"].get<int>()));
                v_box_columns->v_ety_columns[1]->set_text(Glib::ustring::compose("$ %1", json["ingreso"].get<int>()));
                v_box_columns->v_ety_columns[2]->set_text(Glib::ustring::compose("$ %1", json["cambio"].get<int>()));
            }
        });
    } catch (const std::exception& e) {
        g_warning("Error respuesta WS: %s", e.what());
    }
}

