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
            Global::Widget::reveal_toast("Refill detenido", Gtk::MessageType::OTHER);

            v_btn_detener->set_visible(false);
            v_btn_incia->set_visible(true);
        }
        v_dialog->close();
    });
    // v_dialog->set_hide_on_close();
    v_dialog->set_visible();
}

void Refill::poll_alerta_niveles()
{
    std::thread([this]()
    {
        while (true)
        {
            std::this_thread::sleep_for(std::chrono::seconds(5));
            auto db = std::make_unique<Configuracion>();
            auto data = db->get_conf_data(1,1);
            if (data->get_item(0)->m_valor == "1")
                continue;

            if(Global::System::token.empty())
                continue;
            
            auto future = cpr::GetAsync(cpr::Url{Global::System::URL, "validadores/get_dashboard"}, Global::Utility::header);
            Global::Utility::consume_and_do(future,[this](cpr::Response response)
            {
                if (response.status_code == 200) 
                {
                    auto json = nlohmann::json::parse(response.text);
                    alerta_niveles(json["bill"]);
                    alerta_niveles(json["coin"]);
                }
            });
        } 
    }).detach();
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
    v_btn_incia->set_visible(true);

    auto future = cpr::GetAsync(cpr::Url{Global::System::URL, "validadores/get_dashboard"}, Global::Utility::header);
    Global::Utility::consume_and_do(future, [this](cpr::Response response)
    {
        if (response.status_code == 200) 
        {
            auto level = std::make_unique<LevelCash>();
            auto json = nlohmann::json::parse(response.text);
            auto level_coin = level->get_level_cash(json["coin"]);
            auto level_bill = level->get_level_cash(json["bill"]);
            
            auto model_bill = v_tree_reciclador_billetes->get_model();
            actualiza_data(model_bill,level_bill);
            auto model_coin = v_tree_reciclador_monedas->get_model();
            actualiza_data(model_coin,level_coin);

            v_lbl_total->set_text(json["total"].get<std::string>());
            v_lbl_total_billetes->set_text(json["total_billetes"].get<std::string>());
            v_lbl_total_monedas->set_text(json["total_monedas_recy"].get<std::string>());
            v_lbl_total_billetes_cass->set_text(json["total_billetes_cass"].get<std::string>());
            v_lbl_total_parcial_billetes->set_text(json["total_billetes_recy"].get<std::string>());
            v_lbl_total_parcial_monedas->set_text(json["total_monedas_recy"].get<std::string>());
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
    v_btn_incia->set_visible(false);

    ws.connect(Global::System::WS +"/ws/refill",[this]() 
    {
        enviar_datos();
    },
    [this](const std::string& msg) 
    {
        manejar_respuesta_servidor(msg);
    },
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
            auto m_log = log->get_log(j["ticket"]);
            auto ticket = m_log->get_item(0);
            auto faltante = j["Cambio_faltante"].get<int>();

            Global::Widget::reveal_toast(Glib::ustring::compose("<span weight=\"bold\">Refill</span>\n\n"
                                                                        "Total: \t\t$%1\n"
                                                                        "Cambio: \t$%2\n"
                                                                        "Ingreso: \t$%3\n"
                                                                        "Faltante: \t$%4\n", 
                                                                        ticket->m_total, 
                                                                        ticket->m_cambio, 
                                                                        ticket->m_ingreso, 
                                                                        faltante), Gtk::MessageType::OTHER);

            Global::System::imprime_ticket(ticket, faltante);

            on_show_map();
            v_lbl_total_parcial_billetes->set_text(j["billetes"].get<std::string>());
            v_lbl_total_parcial_monedas->set_text(j["monedas"].get<std::string>());
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
                auto m_log = log->get_log(j["ticket"]);
                auto ticket = m_log->get_item(0);

                Global::System::imprime_ticket(ticket, 0);
            }
        }
        Global::Widget::m_refActionGroup->lookup_action("cerrarsesion")->activate(); });
}

void Refill::enviar_datos()
{
    auto json = nlohmann::json
    {
        {"action", "consulta"}
    };
    ws.send(json.dump());
}

void Refill::manejar_respuesta_servidor(const std::string& respuesta)
{
    try 
    {
        auto json = nlohmann::json::parse(respuesta);
        auto status = json["status"].get<std::string>();

        if (status == "idle") {
            Glib::signal_idle().connect_once([this]() {
                ws.close();
                g_info("WebSocket cerrado desde hilo principal");
            });
            return; 
        }
        auto level = std::make_unique<LevelCash>();
        
        auto level_coin = level->get_level_cash(json["coin"]);
        auto level_bill = level->get_level_cash(json["bill"]);
        
        auto model_bill = v_tree_reciclador_billetes->get_model();
        auto model_coin = v_tree_reciclador_monedas->get_model();

        size_t total_recy_billetes = 0;
        size_t total_cass_billetes = 0;
        size_t total_recy_monedas = 0;

        for (size_t i = 0; i < level_coin->get_n_items(); i++)
            total_recy_monedas += level_coin->get_item(i)->m_ingreso * level_coin->get_item(i)->m_denominacion;
        
        for (size_t i = 0; i < level_bill->get_n_items(); i++)
        {
            total_recy_billetes += level_bill->get_item(i)->m_ingreso * level_bill->get_item(i)->m_denominacion;
            total_cass_billetes += level_bill->get_item(i)->m_cant_alm * level_bill->get_item(i)->m_denominacion;
        }

        Glib::signal_idle().connect_once([this, model_coin, level_coin, model_bill, level_bill, total_recy_monedas, total_recy_billetes]() 
        {
            actualiza_data(model_coin,level_coin);
            actualiza_data(model_bill,level_bill);
            v_lbl_total_parcial_billetes->set_text(Glib::ustring::compose("$ %1", total_recy_billetes));
            v_lbl_total_parcial_monedas->set_text(Glib::ustring::compose("$ %1", total_recy_monedas));
        });

        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        enviar_datos();

    } catch (const std::exception& e) {
        g_warning("Error respuesta WS: %s", e.what());
    }
}