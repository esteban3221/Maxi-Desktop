#include "pago_a.hpp"

void PagoA::on_btn_enter_clicked()
{
    if (v_base_nip->v_ety_spin->get_value_as_int() < 1)
    {
        Global::Widget::reveal_toast("El monto a pagar debe ser mayor a 0", Gtk::MessageType::WARNING);
        
        return;
    }

    set_sensitive(false);
    v_base_nip->v_ety_spin->update();

    auto value = v_base_nip->v_ety_spin->get_value_as_int();
    auto json = nlohmann::json{{"value", value},{"concepto", v_ety_concepto.get_text()}};
    auto future = cpr::PostAsync(cpr::Url{Global::System::URL + "accion/inicia_pago"}, 
                                    Global::Utility::header,cpr::Body{json.dump()});

    Global::Utility::consume_and_do(future, [this](const cpr::Response &response)
                                    {
                Global::Widget::v_progress_bar->set_fraction(1.0);

                if (response.status_code == 200) {
                    auto j = nlohmann::json::parse(response.text);

                    
                    auto log = std::make_unique<Log>();
                    auto ticket = log->get_log(j["ticket"])->get_item(0);
                    
                    Global::System::imprime_ticket(ticket);

                    Global::Widget::reveal_toast(Glib::ustring::compose("<span weight=\"bold\">Pago</span>\n\n"
                                                                        "Total: \t\t$%1\n"
                                                                        "Cambio: \t$%2\n"
                                                                        "Ingreso: \t$%3\n"
                                                                        "Estatus \t%4", 
                                                                        ticket->m_total, 
                                                                        ticket->m_cambio, 
                                                                        ticket->m_ingreso, 
                                                                        ticket->m_estatus));

                } else {
                    v_dialog.reset(new Gtk::MessageDialog(*Global::Widget::v_main_window, "Error", false, (Gtk::MessageType)3 /*para windows*/, Gtk::ButtonsType::NONE));
                    v_dialog->set_secondary_text(response.text);
                    v_dialog->set_visible();
                }
                Global::Widget::m_refActionGroup->lookup_action("cerrarsesion")->activate();
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
