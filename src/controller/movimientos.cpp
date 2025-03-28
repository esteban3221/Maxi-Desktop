#include "controller/movimientos.hpp"
#include "movimientos.hpp"

Movimientos::Movimientos(BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &refBuilder) : VMovimientos(cobject, refBuilder)
{
    v_spin_pag->set_adjustment(Gtk::Adjustment::create(1, 1, 100));
    init_datos();

    signal_map().connect(sigc::mem_fun(*this,&Movimientos::consume_data));
}

Movimientos::~Movimientos()
{
}

void Movimientos::init_datos()
{
    std::vector<Glib::ustring> tipos = {"Ninguno", "Venta", "Pago", "Pago Manual", "Refill"};
    m_list_tipos = Gtk::StringList::create(tipos);
    v_dp_tipo->set_model(m_list_tipos);

    auto model_list = Gio::ListStore<MLog>::create();
    auto selection = Gtk::SingleSelection::create(model_list);

    v_column_log->set_model(selection);

    {
        auto factory = Gtk::SignalListItemFactory::create();
        factory->signal_setup().connect(sigc::mem_fun(*this, &Movimientos::on_setup_label));
        factory->signal_bind().connect(sigc::mem_fun(*this, &Movimientos::on_bind_id));
        auto column = Gtk::ColumnViewColumn::create("ID", factory);
        v_column_log->append_column(column);
    }

    {
        auto factory = Gtk::SignalListItemFactory::create();
        factory->signal_setup().connect(sigc::mem_fun(*this, &Movimientos::on_setup_label_text));
        factory->signal_bind().connect(sigc::mem_fun(*this, &Movimientos::on_bind_usuario));
        auto column = Gtk::ColumnViewColumn::create("Usuario", factory);
        column->set_expand();
        v_column_log->append_column(column);
    }

    {
        auto factory = Gtk::SignalListItemFactory::create();
        factory->signal_setup().connect(sigc::mem_fun(*this, &Movimientos::on_setup_label_text));
        factory->signal_bind().connect(sigc::mem_fun(*this, &Movimientos::on_bind_tipo));
        auto column = Gtk::ColumnViewColumn::create("Tipo", factory);
        column->set_expand();
        v_column_log->append_column(column);
    }

    {
        auto factory = Gtk::SignalListItemFactory::create();
        factory->signal_setup().connect(sigc::mem_fun(*this, &Movimientos::on_setup_label));
        factory->signal_bind().connect(sigc::mem_fun(*this, &Movimientos::on_bind_ingreso));
        auto column = Gtk::ColumnViewColumn::create("Ingreso", factory);
        column->set_expand();
        v_column_log->append_column(column);
    }

    {
        auto factory = Gtk::SignalListItemFactory::create();
        factory->signal_setup().connect(sigc::mem_fun(*this, &Movimientos::on_setup_label));
        factory->signal_bind().connect(sigc::mem_fun(*this, &Movimientos::on_bind_cambio));
        auto column = Gtk::ColumnViewColumn::create("Cambio", factory);
        column->set_expand();
        v_column_log->append_column(column);
    }

    {
        auto factory = Gtk::SignalListItemFactory::create();
        factory->signal_setup().connect(sigc::mem_fun(*this, &Movimientos::on_setup_label));
        factory->signal_bind().connect(sigc::mem_fun(*this, &Movimientos::on_bind_total));
        auto column = Gtk::ColumnViewColumn::create("Total", factory);
        column->set_expand();
        v_column_log->append_column(column);
    }

    {
        auto factory = Gtk::SignalListItemFactory::create();
        factory->signal_setup().connect(sigc::mem_fun(*this, &Movimientos::on_setup_label_text));
        factory->signal_bind().connect(sigc::mem_fun(*this, &Movimientos::on_bind_estatus));
        auto column = Gtk::ColumnViewColumn::create("Estatus", factory);
        column->set_expand();
        v_column_log->append_column(column);
    }

    {
        auto factory = Gtk::SignalListItemFactory::create();
        factory->signal_setup().connect(sigc::mem_fun(*this, &Movimientos::on_setup_label_text));
        factory->signal_bind().connect(sigc::mem_fun(*this, &Movimientos::on_bind_fecha));
        auto column = Gtk::ColumnViewColumn::create("Fecha", factory);
        column->set_expand();
        v_column_log->append_column(column);
    }
}

void Movimientos::actualiza_data(const Glib::RefPtr<Gtk::SelectionModel> &selection, const Glib::RefPtr<Gio::ListStore<MLog>> &log)
{
    auto single = std::dynamic_pointer_cast<Gtk::SingleSelection>(selection);
    auto list_store = std::dynamic_pointer_cast<Gio::ListStore<MLog>>(single->get_model());

    list_store->remove_all();

    for (size_t i = 0; i < log->get_n_items(); i++)
    {
        list_store->append(log->get_item(i));
        single->set_selected(i);
    }
    single->unselect_all();
}

void Movimientos::consume_data()
{
    v_spin_pag->update();

    std::thread([this]()
                {
        // auto tipo = v_dp_tipo->get_selected_item();
        auto f_ini = v_ety_ini->get_text();
        auto f_fin = v_ety_fin->get_text();
        auto pag = v_spin_pag->get_value_as_int();

        auto json = nlohmann::json{
            {"tipo", ""},
            {"f_ini", f_ini},
            {"f_fin", f_fin},
            {"pag", 0}};

        auto future = cpr::PostAsync(cpr::Url{Global::System::URL + "log/movimientos"}, cpr::Body{json.dump()});

        while (future.wait_for(std::chrono::milliseconds(100)) != std::future_status::ready)
        {
            Glib::signal_idle().connect_once([this]() { Global::Widget::v_progress_bar->pulse(); });
        }
        auto cpy = future.share();

        Glib::signal_idle().connect_once([this, cpy]()
        {
            Global::Widget::v_progress_bar->set_fraction(1.0);

            auto response = cpy.get();

            if (response.status_code == cpr::status::HTTP_OK) 
            {
                auto j = nlohmann::json::parse(response.text);

                auto log = std::make_unique<Log>();
                auto m_log = log->get_log(j["log"]);

                auto model = v_column_log->get_model();
                actualiza_data(model, m_log);
            }
        }); }).detach();
}
