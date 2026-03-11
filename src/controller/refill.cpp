#include "controller/refill.hpp"

Refill::Refill(BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &refBuilder) : VRefill(cobject, refBuilder)
{
    this->signal_map().connect(sigc::mem_fun(*this, &Refill::on_show_map));
    poll_alerta_niveles();

    auto m_list_bill = Gio::ListStore<MLevelCash>::create();
    auto m_list_coin = Gio::ListStore<MLevelCash>::create();
    init_data(v_tree_reciclador_billetes, m_list_bill);
    init_data(v_tree_reciclador_monedas, m_list_coin);

    v_btn_incia->signal_clicked().connect(sigc::mem_fun(*this, &Refill::on_btn_iniciar));
    v_btn_transpaso->signal_clicked().connect(sigc::mem_fun(*this, &Refill::on_btn_transpaso));
    v_btn_detener->signal_clicked().connect(sigc::mem_fun(*this, &Refill::on_btn_detener));
}

Refill::~Refill()
{
}

void Refill::on_btn_detener()
{
    v_dialog.reset(new Gtk::MessageDialog(*Global::Widget::v_main_window, "Refill", false, Gtk::MessageType::QUESTION, Gtk::ButtonsType::YES_NO, true));
    v_dialog->set_secondary_text("¿Está seguro de detener el proceso de Refill?");
    v_dialog->signal_response().connect([this](int response_id)
    {
        if (response_id == Gtk::ResponseType::YES)
        {
            ws.send(nlohmann::json{{"action", "detener"}}.dump());
            Global::Widget::reveal_toast("Refill detenido");

            v_btn_detener->set_sensitive(false);
            v_btn_incia->set_visible(true);
        }
        v_dialog->close();
    });
    v_dialog->set_visible();
}

void Refill::poll_alerta_niveles()
{
    // std::thread([this]()
    // {
    //     while (true)
    //     {
    //         std::this_thread::sleep_for(std::chrono::seconds(5));
    //         auto db = std::make_unique<Configuracion>();
    //         auto data = db->get_conf_data(1,1);
    //         if (data->get_item(0)->m_valor == "1")
    //             continue;

    //         if(Global::System::token.empty())
    //             continue;
            
    //         auto future = cpr::GetAsync(cpr::Url{Global::System::URL, "validadores/get_dashboard"}, Global::Utility::header);
    //         Global::Utility::consume_and_do(future,[this](cpr::Response response)
    //         {
    //             if (response.status_code == 200) 
    //             {
    //                 auto json = nlohmann::json::parse(response.text);
    //                 alerta_niveles(json["bill"]);
    //                 alerta_niveles(json["coin"]);
    //             }
    //         });
    //     } 
    // }).detach();
}

void Refill::alerta_niveles(const nlohmann::json &json)
{
    for (auto &&i : json)
    {
        auto denominacion = i["Denominacion"].get<int>();

        auto nivel_actual = i["Recyclador"].get<int>();
        auto nivel_minimo = i["Inmovilidad_Min"].get<int>();
        auto nivel_maximo = i["Inmovilidad_Max"].get<int>();

        if (nivel_actual < nivel_minimo)
            Global::Widget::reveal_toast(Glib::ustring::compose("Nivel minimo alcanzado para la denominación de $ %1", denominacion).c_str(), (Gtk::MessageType)3 /* Critico */);
        else if (nivel_actual > nivel_maximo)
            Global::Widget::reveal_toast(Glib::ustring::compose("Nivel maximo alcanzado para la denominación de $ %1", denominacion).c_str(), Gtk::MessageType::WARNING);
    }
}

void Refill::on_show_map()
{
    v_mon_col_ingreso->set_visible(false);
    v_mon_col_ingreso_total->set_visible(false);
    v_bill_col_ingreso->set_visible(false);
    v_bill_col_ingreso_total->set_visible(false);

    v_btn_detener->set_visible(false);
    v_btn_detener->set_sensitive();
    v_btn_incia->set_visible();
    v_btn_incia->set_sensitive();

    auto future = cpr::GetAsync(cpr::Url{Global::System::URL, "validadores/get_dashboard"}, Global::Utility::header);
    Global::Utility::consume_and_do(future, [this](cpr::Response response)
    {
        if (response.status_code == 200) 
        {
            auto level = std::make_unique<LevelCash>();
            auto json = nlohmann::json::parse(response.text);

            long long total_bill_cass = 0;
            long long total_bill_recy = 0;
            long long total_mon_cass  = 0;
            long long total_mon_recy  = 0;

            for (auto it = json.begin(); it != json.end(); ++it)
            {
                auto device_data = it.value();
                auto type = device_data["type"].get<std::string>();
                auto levels = device_data["levels"];

                for (auto& item : levels)
                {
                    long long val = item["value"].get<long long>();
                    int in_cashbox = item["storedInCashbox"].get<int>();
                    int in_payout  = item["storedInPayout"].get<int>();

                    if (type == "BILL") 
                    {
                        total_bill_cass += (val * in_cashbox);
                        total_bill_recy += (val * in_payout);
                    } 
                    else 
                        total_mon_recy  += (val * in_payout);
                    
                }

                auto level_cash = level->get_level_cash(levels);
                auto model = (type == "BILL") ? v_tree_reciclador_billetes->get_model() : v_tree_reciclador_monedas->get_model();
                actualiza_data(model, level_cash);
            }

            double t_bill_cass = total_bill_cass / 100.0;
            double t_bill_recy = total_bill_recy / 100.0;
            double t_mon_cass  = total_mon_cass / 100.0;
            double t_mon_recy  = total_mon_recy / 100.0;

            v_lbl_total_billetes->set_text(Glib::ustring::format(t_bill_cass + t_bill_recy));
            v_lbl_total_monedas->set_text(Glib::ustring::format(t_mon_cass + t_mon_recy));
    
            v_lbl_total_billetes_cass->set_text(Glib::ustring::format(t_bill_cass));
            v_lbl_total_parcial_billetes->set_text(Glib::ustring::format(t_bill_recy));
            
            v_lbl_total_parcial_monedas->set_text(Glib::ustring::format(t_mon_recy));
            v_lbl_total->set_text(Glib::ustring::format(t_bill_cass + t_bill_recy + t_mon_cass + t_mon_recy));
        } 
    });
}

void Refill::init_data(Gtk::ColumnView *vcolumn, const Glib::RefPtr<Gio::ListStore<MLevelCash>> &level)
{
    auto selection_model = Gtk::SingleSelection::create(level);

    safe_clear_column_view(vcolumn);
    vcolumn->set_model(selection_model);
    vcolumn->add_css_class("data-table");

    {
        auto factory = Gtk::SignalListItemFactory::create();
        factory->signal_setup().connect(sigc::mem_fun(*this, &Refill::on_setup_label));
        factory->signal_bind().connect(sigc::mem_fun(*this, &Refill::on_bind_deno));
        auto column = Gtk::ColumnViewColumn::create("Denominacion", factory);
        vcolumn->append_column(column);
    }

    if (vcolumn == v_tree_reciclador_billetes)
    {
        auto factory = Gtk::SignalListItemFactory::create();
        factory->signal_setup().connect(sigc::mem_fun(*this, &Refill::on_setup_label));
        factory->signal_bind().connect(sigc::mem_fun(*this, &Refill::on_bind_alm));
        auto column = Gtk::ColumnViewColumn::create("Casette", factory);
        column->set_expand(true);
        vcolumn->append_column(column);
    }

    {
        auto factory = Gtk::SignalListItemFactory::create();
        factory->signal_setup().connect(sigc::mem_fun(*this, &Refill::on_setup_label));
        factory->signal_bind().connect(sigc::mem_fun(*this, &Refill::on_bind_recy));
        auto column = Gtk::ColumnViewColumn::create("Recylador", factory);
        column->set_expand(true);
        vcolumn->append_column(column);
    }

    {
        auto factory = Gtk::SignalListItemFactory::create();
        factory->signal_setup().connect(sigc::mem_fun(*this, &Refill::on_setup_spin));
        factory->signal_bind().connect(sigc::mem_fun(*this, &Refill::on_bind_inmo_min));
        auto column = Gtk::ColumnViewColumn::create("Inmovilidad Minima", factory);
        column->set_expand(true);
        vcolumn->append_column(column);
    }

    {
        auto factory = Gtk::SignalListItemFactory::create();
        factory->signal_setup().connect(sigc::mem_fun(*this, &Refill::on_setup_spin));
        factory->signal_bind().connect(sigc::mem_fun(*this, &Refill::on_bind_inmo));
        auto column = Gtk::ColumnViewColumn::create("Inmovilidad", factory);
        column->set_expand(true);
        vcolumn->append_column(column);
    }

    {
        auto factory = Gtk::SignalListItemFactory::create();
        factory->signal_setup().connect(sigc::mem_fun(*this, &Refill::on_setup_spin));
        factory->signal_bind().connect(sigc::mem_fun(*this, &Refill::on_bind_inmo_max));
        auto column = Gtk::ColumnViewColumn::create("Inmovilidad Maxima", factory);
        column->set_expand(true);
        vcolumn->append_column(column);
    }

    {
        auto factory = Gtk::SignalListItemFactory::create();
        factory->signal_setup().connect(sigc::mem_fun(*this, &Refill::on_setup_button));
        factory->signal_bind().connect(sigc::mem_fun(*this, &Refill::on_bind_btn));
        auto column = Gtk::ColumnViewColumn::create("Guardar", factory);
        vcolumn->append_column(column);
    }

    {
        auto factory = Gtk::SignalListItemFactory::create();
        factory->signal_setup().connect(sigc::mem_fun(*this, &Refill::on_setup_label));
        factory->signal_bind().connect(sigc::mem_fun(*this, &Refill::on_bind_ingreso));
        auto column = Gtk::ColumnViewColumn::create("Ingreso", factory);
        vcolumn == v_tree_reciclador_billetes ? (v_bill_col_ingreso = column) : (v_mon_col_ingreso = column);
        column->set_visible(false);
        vcolumn->append_column(column);
    }

    {
        auto factory = Gtk::SignalListItemFactory::create();
        factory->signal_setup().connect(sigc::mem_fun(*this, &Refill::on_setup_label));
        factory->signal_bind().connect(sigc::mem_fun(*this, &Refill::on_bind_ingreso_total));
        auto column = Gtk::ColumnViewColumn::create("Total", factory);
        vcolumn == v_tree_reciclador_billetes ? (v_bill_col_ingreso_total = column) : (v_mon_col_ingreso_total = column);
        column->set_visible(false);
        vcolumn->append_column(column);
    }

}

void Refill::actualiza_data(const Glib::RefPtr<Gtk::SelectionModel> &selection, const Glib::RefPtr<Gio::ListStore<MLevelCash>> &level)
{
    auto single = std::dynamic_pointer_cast<Gtk::SingleSelection>(selection);
    auto list_store = std::dynamic_pointer_cast<Gio::ListStore<MLevelCash>>(single->get_model());

    list_store->remove_all();

    for (size_t i = 0; i < level->get_n_items(); i++)
    {
        list_store->append(level->get_item(i));
        single->set_selected(i);
    }
    single->unselect_all();
}

void Refill::safe_clear_column_view(Gtk::ColumnView *column_view)
{
    if (!column_view)
        return;

    try
    {
        auto selection = column_view->get_model();
        auto columns = column_view->get_columns();

        for (size_t i = 0; i < columns->get_n_items(); i++)
        {
            auto col = columns->get_typed_object<Gtk::ColumnViewColumn>(i);
            column_view->remove_column(col);
        }

        if (!selection)
            return;
        if (auto single_sel = std::dynamic_pointer_cast<Gtk::SingleSelection>(selection))
            if (auto list_model = single_sel->get_model())
                if (auto list_store = std::dynamic_pointer_cast<Gio::ListStore<MLevelCash>>(list_model))
                    list_store->remove_all();
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error clearing ColumnView: " << e.what() << std::endl;
    }
}

void Refill::on_btn_iniciar()
{
    v_mon_col_ingreso->set_visible(true);
    v_mon_col_ingreso_total->set_visible(true);
    v_bill_col_ingreso->set_visible(true);
    v_bill_col_ingreso_total->set_visible(true);

    v_btn_detener->set_visible(true);
    v_btn_incia->set_sensitive(false);

    ws.connect(Global::System::WS + "/ws/refill",[this]() 
    {
        enviar_datos();
    },
    sigc::mem_fun(*this, &Refill::manejar_respuesta_servidor),
    [this](const std::string& err) 
    {
        Global::Widget::reveal_toast(Glib::ustring::compose("Error de conexión: %1", err), (Gtk::MessageType)3, 5000);
    },
    [this](int code, const std::string& reason) 
    {
        g_info("Conexión cerrada: %s (código %d)", reason.c_str(), code);
    });

    auto future = cpr::PostAsync(cpr::Url{Global::System::URL + "accion/inicia_refill"}, Global::Utility::header);
    Global::Utility::consume_and_do(future, [this](cpr::Response response)
    {
        if (response.status_code == 200) 
        {
            auto j = nlohmann::json::parse(response.text);

            auto log = std::make_unique<Log>();
            auto ticket = log->get_log(j["ticket"])->get_item(0);
            
            

            Global::Widget::reveal_toast(Glib::ustring::compose("<span weight=\"bold\">Refill</span>\n\n"
                                                                        "Total: \t\t$%1\n"
                                                                        "Cambio: \t$%2\n"
                                                                        "Ingreso: \t$%3\n"
                                                                        "Estatus: \t%4", 
                                                                        ticket->m_total, 
                                                                        ticket->m_cambio, 
                                                                        ticket->m_ingreso, 
                                                                        ticket->m_estatus));

            Global::System::imprime_ticket(ticket);

            on_show_map();
            // v_lbl_total_parcial_billetes->set_text(j["billetes"].get<std::string>());
            // v_lbl_total_parcial_monedas->set_text(j["monedas"].get<std::string>());
            Global::Widget::m_refActionGroup->lookup_action("cerrarsesion")->activate();
        } 
    });
}

void Refill::on_btn_transpaso()
{
    auto future = cpr::PostAsync(cpr::Url{Global::System::URL, "validador/transpaso"}, Global::Utility::header);
    Global::Utility::consume_and_do(future, [this](cpr::Response response)
                                    {
        if (response.status_code != 200) 
        {
            Global::Widget::reveal_toast("Error al realizar el traspaso", Gtk::MessageType(3));
            return;
        }
        else
        {
            auto j = nlohmann::json::parse(response.text);
            if (j.contains("ticket")) 
            {
                auto log = std::make_unique<Log>();
                auto ticket = log->get_log(j["ticket"])->get_item(0);
                

                Global::System::imprime_ticket(ticket);
            }
        }
        Global::Widget::m_refActionGroup->lookup_action("cerrarsesion")->activate(); });
}

void Refill::enviar_datos()
{
}

void Refill::manejar_respuesta_servidor(const std::string& respuesta)
{
    try 
    {
        auto json = nlohmann::json::parse(respuesta);
        auto level = std::make_unique<LevelCash>();
        if(json.contains("status") && json["status"].get<std::string>() == "detenido")
        {
            ws.close();
            return;
        }

        auto it = json.begin(); 
        
        const auto key = it.key();
        auto data = it.value();
        auto is_coin = key == "COIN";

        auto m_item = level->from_json(data);
        auto model = is_coin ? v_tree_reciclador_monedas->get_model() : v_tree_reciclador_billetes->get_model();
        auto single = std::dynamic_pointer_cast<Gtk::SingleSelection>(model);
        auto list_store = std::dynamic_pointer_cast<Gio::ListStore<MLevelCash>>(single->get_model());
        
        for (size_t i = 0; i < list_store->get_n_items(); i++)
        {
            if (auto item = list_store->get_item(i); item->m_denominacion == m_item->m_denominacion)
            {
                Glib::signal_idle().connect_once([this, list_store, single, m_item, i]
                {
                    list_store->remove(i);
                    list_store->insert(i, m_item);
                    single->select_item(i, true);
                });
            }
        }

        size_t total_recy_billetes = 0;
        size_t total_cass_billetes = 0;
        size_t total_recy_monedas = 0;



    } catch (const std::exception& e) {
        g_warning("Error respuesta WS: %s", e.what());
    }
}