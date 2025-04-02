#include "pago_a.hpp"

void PagoA::on_btn_enter_clicked()
{
    if (v_base_nip->v_ety_spin->get_value_as_int() < 1)
    {
        v_dialog.reset(new Gtk::MessageDialog(*Global::Widget::v_main_window, "Error", false,(Gtk::MessageType) 3 /*Gtk::MessageType::ERROR para windows*/));
        v_dialog->set_secondary_text("El monto a vender debe ser mayor a 0");
        v_dialog->set_hide_on_close();
        v_dialog->set_visible();
        return;
    }

    set_sensitive(false);
    v_base_nip->v_ety_spin->update();

    std::thread([this]()
                {
        try {
            
            auto value = v_base_nip->v_ety_spin->get_value_as_int();
            auto json = nlohmann::json{{"value", value}};
            auto future = cpr::PostAsync(cpr::Url{Global::System::URL + "accion/inicia_pago"}, cpr::Body{json.dump()});

            // Espera la respuesta con un timeout
            while (future.wait_for(std::chrono::milliseconds(100)) != std::future_status::ready) {
                // Actualiza la barra de progreso de manera segura
                Glib::signal_idle().connect_once([this]() {
                    Global::Widget::v_progress_bar->pulse();
                });
            }

            // Guarda la respuesta en una variable local
            auto response = future.get();  // Solo se llama a .get() una vez

            // Procesa la respuesta en el hilo de GTK
            Glib::signal_idle().connect_once([this, response]() {
                Global::Widget::v_progress_bar->set_fraction(1.0);

                if (response.status_code == cpr::status::HTTP_OK) {
                    // auto j = nlohmann::json::parse(response.text);
                    
                    v_box_columns->v_ety_columns[0]->set_text(v_base_nip->v_ety_spin->get_text());
                    v_box_columns->v_ety_columns[1]->set_text("0");
                    v_box_columns->v_ety_columns[2]->set_text("0");

                    // if (j["Cambio_faltante"].get<int>() > 0) {
                    //     v_dialog.reset(new Gtk::MessageDialog(*Global::Widget::v_main_window, "Cambio Faltante", false, Gtk::MessageType::INFO));
                    //     v_dialog->set_secondary_text("Se requiere un cambio de " + j["Cambio_faltante"].get<std::string>());
                    //     v_dialog->set_visible();
                    // }
                } else {
                    v_dialog.reset(new Gtk::MessageDialog(*Global::Widget::v_main_window, "Error", false, (Gtk::MessageType)3 /*Gtk::MessageType::ERROR para windows*/));
                    v_dialog->set_secondary_text(response.text);
                    v_dialog->set_visible();
                }

                set_sensitive(true);
            });
        } catch (const std::exception& e) {
            Glib::signal_idle().connect_once([this, e]() {
                v_dialog.reset(new Gtk::MessageDialog(*Global::Widget::v_main_window, "Error", false,(Gtk::MessageType)3 /*Gtk::MessageType::ERROR para windows*/));
                v_dialog->set_secondary_text("Excepción: " + std::string(e.what()));
                v_dialog->set_visible();
                set_sensitive(true);
            });
        } })
        .detach();
}

PagoA::PagoA(/* args */)
{
    v_base_nip->v_btn_nip_enter->signal_clicked().connect(sigc::mem_fun(*this, &PagoA::on_btn_enter_clicked));
    v_base_nip->v_ety_spin->set_text("");
    v_base_nip->v_ety_spin->signal_activate().connect([this](){v_base_nip->v_btn_nip_enter->grab_focus();});
    this->signal_map().connect(sigc::mem_fun(*this, &PagoA::on_map_show));
}

PagoA::~PagoA()
{
}

void PagoA::on_map_show()
{
    v_base_nip->v_ety_spin->grab_focus();
    v_base_nip->v_ety_spin->set_text("");
}
