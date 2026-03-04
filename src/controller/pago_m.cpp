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
    total = 0;

    Global::Widget::reveal_toast("Solicitando datos a validadores, Espere un momento.");

    for (auto &&i : v_box_level_validadores->get_children())
            v_box_level_validadores->remove(*i);

    auto future = cpr::GetAsync(cpr::Url{Global::System::URL, "log/get_levels"}, Global::Utility::header);
    Global::Utility::consume_and_do(future, [this](cpr::Response response)
    {
        if (response.status_code == 200) 
        {
            std::vector<std::pair<size_t,size_t>> vec_val;
            auto json = nlohmann::json::parse(response.text);
            for (auto &&i : json.items())
            {
                for (auto &&j : i.value())
                {
                    vec_val.push_back({j["value"].get<int>() / 100, j["storedInPayout"].get<int>()});
                }
                v_box_level_validadores->append(*agregar_contenedor(vec_val, i.key()));
                vec_val.clear();
            }

            for (auto const& [device_id, data_list] : m_inputs_pago) 
            {
                for (auto const& item : data_list) 
                    item.spin->signal_changed().connect(sigc::mem_fun(*this,&PagoM::on_spin_value_changed));
            }
        } 
    });
}

void PagoM::on_spin_value_changed()
{
    total = 0;

    for (auto const& [device_id, data_list] : m_inputs_pago) 
    {
        for (auto const& item : data_list) 
            total += (item.spin->get_value_as_int() * item.denominacion);
    }

    v_btn_cobrar->set_label(Glib::ustring::compose("Pagar $ %1", total));
}

void PagoM::on_btn_cobrar_clicked()
{
    on_spin_value_changed();

    if (total > 0)
    {
        nlohmann::json envio;
        envio["concepto"] = v_ety_concepto->get_text();
        envio["total"] = total;
        for (auto const& [device_id, spins] : m_inputs_pago) 
        {
            std::vector<int> cantidades;
            for (auto const& s : spins) cantidades.push_back(s.spin->get_value_as_int());
            
            envio["pago_manual"][device_id] = cantidades;
        }

        auto future = cpr::PostAsync(cpr::Url{Global::System::URL + "accion/inicia_pago_manual"}, Global::Utility::header, cpr::Body{envio.dump()} );

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
                v_dialog.reset(new Gtk::MessageDialog(*Global::Widget::v_main_window,"Info",false,Gtk::MessageType::INFO, Gtk::ButtonsType::NONE));
                v_dialog->set_secondary_text(response.text);
                v_dialog->set_visible();
            }
            Global::Widget::m_refActionGroup->lookup_action("cerrarsesion")->activate(); 
        });
    }
    else
    {
        v_dialog.reset(new Gtk::MessageDialog(*Global::Widget::v_main_window, "Info", false, Gtk::MessageType::INFO, Gtk::ButtonsType::NONE));
        v_dialog->set_secondary_text("El monto a vender debe ser mayor a 0");
        v_dialog->set_visible();
    }
}