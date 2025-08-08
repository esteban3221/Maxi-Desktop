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

}

Refill::~Refill()
{
}

void Refill::poll_alerta_niveles()
{
    std::thread([this]()
    {
        while (true)
        {
            if(Global::System::token.empty())
                continue;
            std::this_thread::sleep_for(std::chrono::seconds(5));
            
            auto future = cpr::GetAsync(cpr::Url{Global::System::URL, "validadores/get_dashboard"}, Global::Utility::header);
            Global::Utility::consume_and_do(future,[this](cpr::Response response)
            {
                if (response.status_code == 200) 
                {
                    auto json = nlohmann::json::parse(response.text);

                    for (auto &&i : json["bill"])
                    {
                        auto denominacion = i["Denominacion"].get<int>();

                        auto nivel_actual = i["Recyclador"].get<int>();
                        auto nivel_minimo = i["Inmovilidad_Min"].get<int>();
                        auto nivel_maximo = i["Inmovilidad_Max"].get<int>();

                        if(nivel_actual < nivel_minimo)
                            Global::Widget::reveal_toast(Glib::ustring::compose("Nivel minimo alcanzado para billetes de $ %1", denominacion).c_str(), (Gtk::MessageType)3 /* Critico */);
                        else if(nivel_actual > nivel_maximo)
                            Global::Widget::reveal_toast(Glib::ustring::compose("Nivel maximo alcanzado para billetes de $ %1", denominacion).c_str(), Gtk::MessageType::WARNING);
                    }
                }
            });
        }
    }).detach();
}

void Refill::on_show_map()
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

    if(vcolumn == v_tree_reciclador_billetes)
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

    // {
    //     auto factory = Gtk::SignalListItemFactory::create();
    //     factory->signal_setup().connect(sigc::mem_fun(*this, &Refill::on_setup_label));
    //     factory->signal_bind().connect(sigc::mem_fun(*this, &Refill::on_bind_ingreso));
    //     auto column = Gtk::ColumnViewColumn::create("Ingreso", factory);
    //     vcolumn->append_column(column);
    // }
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
    auto future = cpr::PostAsync(cpr::Url{Global::System::URL + "accion/inicia_refill"}, Global::Utility::header);
    Global::Utility::consume_and_do(future,[this](cpr::Response response)
    {
        if (response.status_code == 200) 
        {
            auto j = nlohmann::json::parse(response.text);

            auto log = std::make_unique<Log>();
            auto m_log = log->get_log(j["ticket"]);
            auto ticket = m_log->get_item(0);
            auto faltante = j["Cambio_faltante"].get<int>();

            Global::Widget::reveal_toast(Glib::ustring::compose("<span weight=\"bold\">Refill</span>\n\n"
                                                                        "Total: $%1\n"
                                                                        "Cambio: $%2\n"
                                                                        "Ingreso: $%3\n"
                                                                        "Faltante: $%4\n", 
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
    auto future = cpr::PostAsync(cpr::Url{Global::System::URL , "validador/transpaso"}, Global::Utility::header);
    Global::Utility::consume_and_do(future,[this](cpr::Response response)
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
        Global::Widget::m_refActionGroup->lookup_action("cerrarsesion")->activate();
    });
}
