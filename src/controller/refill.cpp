#include "controller/refill.hpp"

Refill::Refill(BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &refBuilder) : VRefill(cobject, refBuilder)
{
    this->signal_map().connect(sigc::mem_fun(*this, &Refill::on_show_map));

    auto m_list_bill = Gio::ListStore<MLevelCash>::create();
    auto m_list_coin = Gio::ListStore<MLevelCash>::create();
    init_data(v_tree_reciclador_billetes, m_list_bill);
    init_data(v_tree_reciclador_monedas, m_list_coin);

    v_btn_incia->signal_clicked().connect(sigc::mem_fun(*this, &Refill::on_btn_iniciar));
    v_btn_retirada->signal_clicked().connect(sigc::mem_fun(*this, &Refill::on_btn_retirada));
    v_btn_transpaso->signal_clicked().connect(sigc::mem_fun(*this, &Refill::on_btn_transpaso));

}

Refill::~Refill()
{
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
                v_lbl_total_monedas->set_text(json["total_monedas"].get<std::string>());
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
        factory->signal_bind().connect(sigc::mem_fun(*this, &Refill::on_bind_inmo));
        auto column = Gtk::ColumnViewColumn::create("Inmovilidad", factory);
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

            Global::System::imprime_ticket(ticket, faltante);

            on_show_map();
            v_lbl_total_parcial_billetes->set_text(j["billetes"].get<std::string>());
            v_lbl_total_parcial_monedas->set_text(j["monedas"].get<std::string>());

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
            Global::Widget::reveal_toast("Error al realizar el traspaso", Gtk::MessageType::ERROR);
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
    });
}

void Refill::on_btn_retirada()
{
    auto future = cpr::PostAsync(cpr::Url{Global::System::URL + "validador/retirada"}, Global::Utility::header);
    Global::Widget::reveal_toast("Retire el casette de billetes o cancele desde POS", Gtk::MessageType::WARNING);
    Global::Utility::consume_and_do(future,[this](cpr::Response response)
    {
        if (response.status_code != 200) 
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

        Global::Widget::v_revealer->set_reveal_child(false);
    });
}
