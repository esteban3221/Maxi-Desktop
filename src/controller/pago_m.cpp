#include "controller/pago_m.hpp"
#include "pago_m.hpp"


PagoM::PagoM(BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &refBuilder) : VPagoM(cobject, refBuilder)
{
    for (auto &&i : v_spin_coin)
        i->signal_value_changed().connect(sigc::mem_fun(*this, &PagoM::on_spin_value_changed));
    for (auto &&i : v_spin_bill)
        i->signal_value_changed().connect(sigc::mem_fun(*this, &PagoM::on_spin_value_changed));

    v_btn_cobrar->signal_clicked().connect(sigc::mem_fun(*this, &PagoM::on_btn_cobrar_clicked));
    signal_map().connect(sigc::mem_fun(*this, &PagoM::on_show_map));
}

PagoM::~PagoM()
{
}

void PagoM::on_show_map()
{
    auto future = cpr::GetAsync(cpr::Url{Global::System::URL, "validadores/get_dashboard"}, Global::Utility::header);
    Global::Utility::consume_and_do(future,[this](cpr::Response response)
    {
        if (response.status_code == 200) 
        {
            auto level = std::make_unique<LevelCash>();
            auto json = nlohmann::json::parse(response.text);
            auto level_coin = level->get_level_cash(json["coin"]);
            auto level_bill = level->get_level_cash(json["bill"]);

            for (size_t i = 0; i < 4; i++)
            {
                auto it = level_coin->get_item(i);
                v_spin_coin[i]->set_adjustment(Gtk::Adjustment::create(0,0,it->m_cant_recy));
                v_lbl_coin[i]->set_text(Glib::ustring::compose("$%1 | %2", it->m_denominacion, it->m_cant_recy));
            }

            for (size_t i = 0; i < 6; i++)
            {
                auto it = level_bill->get_item(i);
                v_lbl_bill[i]->set_text(Glib::ustring::compose("$%1 | %2", it->m_denominacion, it->m_cant_recy));
                v_spin_bill[i]->set_adjustment(Gtk::Adjustment::create(0,0,it->m_cant_recy));
            }
        }
    });
    
}

void PagoM::on_spin_value_changed()
{
    total = 0;
    for (size_t i = 0; i < 4; i++)
        total += coins.at(i) * v_spin_coin[i]->get_value_as_int();

    for (size_t i = 0; i < 6; i++)
        total += bills.at(i) * v_spin_bill[i]->get_value_as_int();

    v_btn_cobrar->set_label(Glib::ustring::compose("Cobrar $ %1", total));
}

void PagoM::on_btn_cobrar_clicked()
{
    total = 0;
    for (size_t i = 0; i < 4; i++)
        total += coins.at(i) * v_spin_coin[i]->get_value_as_int();

    for (size_t i = 0; i < 6; i++)
        total += bills.at(i) * v_spin_bill[i]->get_value_as_int();

    if (total > 0)
    {

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
            {"total", total}};

        auto future = cpr::PostAsync(cpr::Url{Global::System::URL + "accion/inicia_pago_manual"}, Global::Utility::header, cpr::Body{json.dump()});

        Global::Utility::consume_and_do(future, [this](const cpr::Response &response)
                                        {
                                            set_sensitive(false);
                                            if (response.status_code == 500)
                                            {
                                                v_dialog.reset(new Gtk::MessageDialog(*Global::Widget::v_main_window,"Info",false,Gtk::MessageType::INFO, Gtk::ButtonsType::NONE));
                                                v_dialog->set_secondary_text(response.text);
                                                v_dialog->set_visible();
                                            }
                                            else if (response.status_code == 200)
                                            {
                                                auto j = nlohmann::json::parse(response.text);
                                                auto log = std::make_unique<Log>();

                                                auto m_log = log->get_log(j["ticket"]);
                                                auto ticket = m_log->get_item(0);
                                                auto faltante = j["Cambio_faltante"].get<int>();
                                                Global::System::imprime_ticket(ticket, faltante);
                                                Global::Widget::reveal_toast(Glib::ustring::compose("<span weight=\"bold\">Pago Manual</span>\n\n"
                                                                        "Total: $%1\n"
                                                                        "Cambio: $%2\n"
                                                                        "Ingreso: $%3\n"
                                                                        "Faltante: $%4", 
                                                                        ticket->m_total, 
                                                                        ticket->m_cambio, 
                                                                        ticket->m_ingreso, 
                                                                        faltante).c_str(), Gtk::MessageType::OTHER);

                                            }
                                            else
                                            {
                                                v_dialog.reset(new Gtk::MessageDialog(*Global::Widget::v_main_window,"Info",false,Gtk::MessageType::INFO, Gtk::ButtonsType::NONE));
                                                v_dialog->set_secondary_text(response.text);
                                                v_dialog->set_visible();
                                            }

                                            set_sensitive(true);

                                            // Procesa la respuesta en el hilo de GTK
                                        });

        }else
        {
            v_dialog.reset(new Gtk::MessageDialog(*Global::Widget::v_main_window,"Info",false,Gtk::MessageType::INFO, Gtk::ButtonsType::NONE));
            v_dialog->set_secondary_text("El monto a vender debe ser mayor a 0");
            v_dialog->set_visible();
        }
        Global::Widget::m_refActionGroup->lookup_action("cerrarsesion")->activate();
    }