#include "cambio_m.hpp"

CambioM::CambioM(BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &refBuilder)
    : VPagoM(cobject, refBuilder)
{

    // View
    v_btn_cobrar->set_label("Iniciar");
    v_btn_aceptar.set_label("Dar Cambio");
    v_btn_cancelar.set_label("Cancelar");
    // v_btn_aceptar.property_halign() = Gtk::Align::CENTER;
    // v_btn_aceptar.property_valign() = Gtk::Align::CENTER;
    // v_btn_aceptar.property_vexpand() = true;
    v_btn_aceptar.set_css_classes({"suggested-action"});
    v_btn_cancelar.set_css_classes({"destructive-action"});
    v_box_botones.set_halign(Gtk::Align::CENTER);
    v_box_botones.append(v_btn_aceptar);
    v_box_botones.append(v_btn_cancelar);
    append(v_box_botones);

    //Controller
    this->signal_map().connect(sigc::mem_fun(*this, &CambioM::on_show_map));
    v_btn_cobrar->signal_clicked().connect(sigc::mem_fun(*this, &CambioM::on_btn_cobrar_clicked));
    v_btn_aceptar.signal_clicked().connect(sigc::mem_fun(*this, &CambioM::on_btn_aceptar_clicked));
    v_btn_cancelar.signal_clicked().connect(sigc::mem_fun(*this, &CambioM::on_btn_cancelar_clicked));

    for (auto &&i : v_spin_coin)
        i->signal_value_changed().connect(sigc::mem_fun(*this, &CambioM::on_spin_value_changed));
    for (auto &&i : v_spin_bill)
        i->signal_value_changed().connect(sigc::mem_fun(*this, &CambioM::on_spin_value_changed));
}

void CambioM::on_show_map()
{
    for (auto &&i : v_spin_bill)
        i->set_sensitive(false);

    for (auto &&i : v_spin_coin)
        i->set_sensitive(false);

    v_btn_aceptar.set_sensitive(false);
    v_btn_cancelar.set_sensitive(false);
    v_btn_cobrar->set_sensitive(true);
    ingreso = total = 0;
}

CambioM::~CambioM()
{
}

void CambioM::activa_spinners(Gtk::SpinButton *spinner[], int pos, int tam)
{
    if (pos == -1)
        for (int i = 0; i < tam; i++)
            spinner[i]->set_sensitive(true);
    else
        for (int i = 0; i < tam; i++)
            if (i <= pos)
                spinner[i]->set_sensitive(true);
            else
                spinner[i]->set_sensitive(false);
}

void CambioM::on_spin_value_changed()
{
    total = 0;
    for (size_t i = 0; i < 4; i++)
        total += coins.at(i) * v_spin_coin[i]->get_value_as_int();

    for (size_t i = 0; i < 6; i++)
        total += bills.at(i) * v_spin_bill[i]->get_value_as_int();

    v_btn_aceptar.set_label(Glib::ustring::compose("Cambio $ %1", total));
}

void CambioM::on_btn_cobrar_clicked()
{
    v_btn_cobrar->set_sensitive(false);
    v_btn_cobrar->set_label("Espere...");
    nlohmann::json json = {{"concepto", v_ety_concepto->get_text()}};
    auto future = cpr::PostAsync(cpr::Url{Global::System::URL + "accion/inicia_cambio_manual"}, Global::Utility::header, cpr::Body{json.dump()});

    Global::Utility::consume_and_do(future, [this](const cpr::Response &response)
                                    {
                                        if (response.status_code == 200)
                                        {
                                            auto j = nlohmann::json::parse(response.text);                            
                                            ingreso = j["ingresado"].get<int>();
                                            int pos_coin = j["pos_coin"].get<int>();
                                            int pos_bill = j["pos_bill"].get<int>();

                                            activa_spinners(v_spin_bill, pos_bill, 6);
                                            activa_spinners(v_spin_coin, pos_coin, 4);

                                            v_btn_aceptar.set_sensitive(true);
                                            v_btn_cancelar.set_sensitive(true);
                                            
                                        } });
}

void CambioM::on_btn_aceptar_clicked()
{
    if (ingreso == total)
    {
        v_btn_aceptar.set_sensitive(false);
        v_btn_cancelar.set_sensitive(false);
        auto json = nlohmann::json{
            {"bill", nlohmann::json::array({v_spin_bill[0]->get_value_as_int(),
                                            v_spin_bill[1]->get_value_as_int(),
                                            v_spin_bill[2]->get_value_as_int(),
                                            v_spin_bill[3]->get_value_as_int(),
                                            v_spin_bill[4]->get_value_as_int(),
                                            v_spin_bill[5]->get_value_as_int()})},
            {"coin", nlohmann::json::array({v_spin_coin[0]->get_value_as_int(),
                                            v_spin_coin[1]->get_value_as_int(),
                                            v_spin_coin[2]->get_value_as_int(),
                                            v_spin_coin[3]->get_value_as_int()})},
            {"total", total},
            {"concepto", v_ety_concepto->get_text()}};

        auto future = cpr::PostAsync(cpr::Url{Global::System::URL + "accion/termina_cambio_manual"},
                                     Global::Utility::header,
                                     cpr::Body{json.dump()});

        Global::Utility::consume_and_do(future, [this](const cpr::Response &response)
                                        {
                                        if (response.status_code == 200)
                                            {
                                                auto j = nlohmann::json::parse(response.text);
                                                auto log = std::make_unique<Log>();

                                                auto m_log = log->get_log(j["ticket"]);
                                                auto ticket = m_log->get_item(0);
                                                auto faltante = j["Cambio_faltante"].get<int>();
                                                Global::System::imprime_ticket(ticket, faltante);
                            
                                                Global::Widget::reveal_toast(Glib::ustring::compose("<span weight=\"bold\">Cambio Manual</span>\n\n"
                                                                        "Total: \t\t$%1\n"
                                                                        "Cambio: \t$%2\n"
                                                                        "Ingreso: \t$%3\n"
                                                                        "Faltante: \t$%4", 
                                                                        ticket->m_total, 
                                                                        ticket->m_cambio, 
                                                                        ticket->m_ingreso, 
                                                                        faltante), Gtk::MessageType::OTHER);
                                                
                                            }
                                            else
                                            {
                                                v_dialog.reset(new Gtk::MessageDialog(*Global::Widget::v_main_window,"Info",false,Gtk::MessageType::INFO, Gtk::ButtonsType::NONE));
                                                v_dialog->set_secondary_text(response.text);
                                                v_dialog->set_visible();
                                            }
                                            Global::Widget::m_refActionGroup->lookup_action("cerrarsesion")->activate(); });
    }
    else
    {
        v_dialog.reset(new Gtk::MessageDialog(*Global::Widget::v_main_window, "Info", false, Gtk::MessageType::INFO, Gtk::ButtonsType::NONE));
        v_dialog->set_secondary_text("El total ingresado no coincide con el cambio a dar");
        v_dialog->set_hide_on_close();
        v_dialog->set_visible();
    }
}

void CambioM::on_btn_cancelar_clicked()
{
    auto future = cpr::PostAsync(cpr::Url{Global::System::URL + "accion/cancelar_cambio_manual"},
                                 Global::Utility::header);

    Global::Utility::consume_and_do(future, [this](const cpr::Response &response)
                                    {
                                        if (response.status_code == 200)
                                                Global::Widget::reveal_toast("Cambio Manual Cancelado", Gtk::MessageType::OTHER);
                                            
                                        Global::Widget::m_refActionGroup->lookup_action("cerrarsesion")->activate(); });
}