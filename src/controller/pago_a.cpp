#include "pago_a.hpp"

void PagoA::on_btn_enter_clicked()
{
    if (v_base_nip->v_ety_spin->get_value_as_int() < 1)
    {
        Global::Widget::v_revealer_title->set_text("El monto a pagar debe ser mayor a 0");
        Global::Widget::v_revealer->set_reveal_child(true);
        return;
    }

    set_sensitive(false);
    v_base_nip->v_ety_spin->update();

    auto value = v_base_nip->v_ety_spin->get_value_as_int();
    auto json = nlohmann::json{{"value", value}};
    auto future = cpr::PostAsync(cpr::Url{Global::System::URL + "accion/inicia_pago"}, cpr::Body{json.dump()});

    Global::Utility::consume_and_do(future, [this](const cpr::Response &response)
                                    {
                Global::Widget::v_progress_bar->set_fraction(1.0);

                if (response.status_code == 200) {
                    auto j = nlohmann::json::parse(response.text);
                    
                    v_box_columns->v_ety_columns[0]->set_text(v_base_nip->v_ety_spin->get_text());
                    v_box_columns->v_ety_columns[1]->set_text("0");
                    v_box_columns->v_ety_columns[2]->set_text("0");

                    auto faltante = j["Cambio_faltante"].get<int>();
                    auto log = std::make_unique<Log>();
                    auto m_log = log->get_log(j["ticket"]);
                    auto ticket = m_log->get_item(0);
                    Global::System::imprime_ticket(ticket, faltante);

                    if (faltante > 0) {
                        v_dialog.reset(new Gtk::MessageDialog(*Global::Widget::v_main_window, "Cambio Faltante", false, Gtk::MessageType::INFO));
                        v_dialog->set_secondary_text("Se requiere un cambio de " + j["Cambio_faltante"].get<std::string>());
                        v_dialog->set_visible();
                    }
                } else {
                    v_dialog.reset(new Gtk::MessageDialog(*Global::Widget::v_main_window, "Error", false, (Gtk::MessageType)3 /*Gtk::MessageType::ERROR para windows*/));
                    v_dialog->set_secondary_text(response.text);
                    v_dialog->set_visible();
                }

                set_sensitive(true); });
}

PagoA::PagoA(/* args */)
{
    v_base_nip->v_btn_nip_enter->signal_clicked().connect(sigc::mem_fun(*this, &PagoA::on_btn_enter_clicked));
    v_base_nip->v_ety_spin->set_text("");
    v_base_nip->v_ety_spin->signal_activate().connect([this]()
                                                      { v_base_nip->v_btn_nip_enter->grab_focus(); });
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
