#include "controller/refill.hpp"
#include "refill.hpp"

Refill::Refill(BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &refBuilder) : VRefill(cobject, refBuilder)
{
    this->signal_map().connect(sigc::mem_fun(*this, &Refill::on_show_map));

    auto m_list_bill = Gio::ListStore<MLevelCash>::create();
    auto m_list_coin = Gio::ListStore<MLevelCash>::create();
    init_data(v_tree_reciclador_billetes, m_list_bill);
    init_data(v_tree_reciclador_monedas, m_list_coin);
}

Refill::~Refill()
{
}

void Refill::on_show_map()
{
    std::thread([this]()
                {
                    auto future = cpr::GetAsync(cpr::Url{Global::System::URL + "validadores/get_dashboard"});
                    while (future.wait_for(std::chrono::milliseconds(100)) != std::future_status::ready)
                    {
                        Glib::signal_idle().connect_once([this]() { Global::Widget::v_progress_bar->pulse(); });
                    }

                    auto cpy = future.share();

                    Glib::signal_idle().connect_once([this, cpy]()
                                                     {
            auto response = cpy.get();

            if (response.status_code == cpr::status::HTTP_OK) 
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
            } });
                }).detach();
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
        {
            if (auto list_model = single_sel->get_model())
            {
                if (auto list_store = std::dynamic_pointer_cast<Gio::ListStore<MLevelCash>>(list_model))
                {
                    list_store->remove_all();
                }
            }
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error clearing ColumnView: " << e.what() << std::endl;
    }
}
