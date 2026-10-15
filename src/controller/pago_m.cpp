#include "controller/pago_m.hpp"

PagoM::PagoM(BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &refBuilder) : VPagoM(cobject, refBuilder)
{
    v_btn_cobrar->signal_clicked().connect(sigc::mem_fun(*this, &PagoM::on_btn_cobrar_clicked));
    signal_map().connect(sigc::mem_fun(*this, &PagoM::on_show_map));
}

PagoM::~PagoM()
{
}

void PagoM::on_show_map()
{
    v_ety_concepto->set_text("");
    m_inputs_pago.clear();
    v_btn_cobrar->set_sensitive();

    total = 0;

    for (auto &&i : v_box_level_validadores->get_children())
        v_box_level_validadores->remove(*i);

    auto future = cpr::GetAsync(cpr::Url{Global::System::URL, "log/get_levels"}, Global::Utility::header);

    Global::Utility::consume_and_do(future, [this](cpr::Response response)
    {
        if (response.status_code != 200)
        {
            Global::Widget::reveal_toast("No tiene permisos para consultar los niveles.", Gtk::MessageType::ERROR, 3000);
            return;
        }

        auto json = nlohmann::json::parse(response.text);

        for (auto &&i : json.items())
        {
            std::vector<std::pair<size_t, size_t>> vec_payout;
            std::vector<std::pair<size_t, size_t>> vec_cashbox;

            bool tiene_datos_cashbox = false;

            if (i.value().contains("levels") && i.value()["levels"].is_array())
            {
                for (auto &&j : i.value()["levels"])
                {
                    size_t denominacion = j["value"].get<int>() / 100;
                    size_t stored_payout = j["storedInPayout"].is_null() ? 0 : j["storedInPayout"].get<size_t>();

                    vec_payout.push_back({denominacion, stored_payout});

                    if (!j["storedInCashbox"].is_null())
                    {
                        tiene_datos_cashbox = true;
                        vec_cashbox.push_back({denominacion, j["storedInCashbox"].get<size_t>()});
                    }
                }
            }

            v_box_level_validadores->append(*agregar_contenedor(vec_payout, i.key()));
        }

        for (auto const &[device_id, data_list] : m_inputs_pago)
        {
            for (auto const &item : data_list)
                item.spin->signal_value_changed().connect(sigc::mem_fun(*this, &PagoM::on_spin_value_changed));
        }
    });
}

void PagoM::on_spin_value_changed()
{
    total = 0;

    for (auto const &[device_id, data_list] : m_inputs_pago)
    {
        for (auto const &item : data_list)
            total += (item.spin->get_value_as_int() * item.denominacion);
    }

    v_btn_cobrar->set_label(Glib::ustring::compose("Pagar $ %1", total));
}

void PagoM::on_btn_cobrar_clicked()
{
    on_spin_value_changed();
    v_btn_cobrar->set_sensitive(false);

    if (total > 0)
    {
        nlohmann::json envio;
        envio["concepto"] = v_ety_concepto->get_text();
        envio["total"] = total;
        for (auto const &[device_id, spins] : m_inputs_pago)
        {
            std::vector<int> cantidades;
            for (auto const &s : spins)
                cantidades.push_back(s.spin->get_value_as_int());

            envio["pago_manual"][device_id] = cantidades;
        }

        auto future = cpr::PostAsync(cpr::Url{Global::System::URL + "accion/inicia_pago_manual"}, Global::Utility::header, cpr::Body{envio.dump()});

        Global::Utility::consume_and_do(future, [this](const cpr::Response &response)
                                        {
                                            if (response.status_code == 200)
                                            {
                                                auto j = nlohmann::json::parse(response.text);
                                                auto log = std::make_unique<Log>();
                                                auto ticket = log->get_log(j["ticket"])->get_item(0);

                                                Global::Widget::reveal_toast(Glib::ustring::compose("<span weight=\"bold\">Pago Manual</span>\n\n"
                                                                                                    "Total: \t\t$%1\n"
                                                                                                    "Cambio: \t$%2\n"
                                                                                                    "Ingreso: \t$%3\n"
                                                                                                    "Estatus \t%4",
                                                                                                    ticket->m_total,
                                                                                                    ticket->m_cambio,
                                                                                                    ticket->m_ingreso,
                                                                                                    ticket->m_estatus));

                                                Global::System::imprime_ticket(ticket);
                                            }
                                            else
                                            {
                                                auto v_dialog = Gtk::AlertDialog::create("Error");
                                                v_dialog->set_detail(response.text);
                                                v_dialog->show(*Global::Widget::v_main_window);
                                            }
                                            Global::Widget::m_refActionGroup->lookup_action("cerrarsesion")->activate();
                                        });
    }
    else
    {
        auto v_dialog = Gtk::AlertDialog::create("Info");
        v_dialog->set_detail("El monto a vender debe ser mayor a 0");
        v_dialog->show(*Global::Widget::v_main_window);
    }
}