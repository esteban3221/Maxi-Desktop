#include "controller/movimientos.hpp"

Movimientos::Movimientos(BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &refBuilder) : VMovimientos(cobject, refBuilder)
{
    v_spin_pag->set_adjustment(Gtk::Adjustment::create(1, 1, 100));
    init_datos();

    signal_map().connect(sigc::mem_fun(*this, &Movimientos::consume_data));
    v_btn_aplica_filtro->signal_clicked().connect(sigc::mem_fun(*this, &Movimientos::consume_data));
    v_btn_imprime_corte->signal_clicked().connect(sigc::mem_fun(*this, &Movimientos::imprime_corte));
    v_ety_ini->signal_icon_press().connect(sigc::mem_fun(*this, &Movimientos::muestra_calendario_inicio));
    v_ety_fin->signal_icon_press().connect(sigc::mem_fun(*this, &Movimientos::muestra_calendario_fin));
    v_calendario.signal_day_selected().connect(sigc::mem_fun(*this, &Movimientos::set_fecha));
    v_btn_remueve_filtros->signal_clicked().connect(sigc::mem_fun(*this, &Movimientos::borra_filtro));
    v_btn_printer->signal_clicked().connect(sigc::mem_fun(*this, &Movimientos::reimprime_tickets));
}

Movimientos::~Movimientos()
{
    v_pop_calendario.unparent();
}

void Movimientos::imprime_corte()
{
    auto id_tipo = v_dp_tipo->get_selected();
    auto tipo = m_list_tipos->get_string(id_tipo);

    v_dialog.reset(new Gtk::MessageDialog(*Global::Widget::v_main_window, "Atención", false, Gtk::MessageType::QUESTION, Gtk::ButtonsType::OK_CANCEL, true));
    v_dialog->set_secondary_text("¿Desea imprimir el tipo de movimiento \"" + tipo + "\" en el corte de caja?");
    v_dialog->signal_response().connect([this,tipo](int response)
    {
        if (Gtk::ResponseType::OK == response)
        {
            auto json = nlohmann::json{{"tipo", tipo}};

            auto future = cpr::GetAsync(cpr::Url{Global::System::URL + "log/corte_caja"}, Global::Utility::header, cpr::Body{json.dump()});
            Global::Utility::consume_and_do(future, [this](const cpr::Response &response)
            {
                if (response.status_code == 200)
                {
                    auto j = nlohmann::json::parse(response.text);

                    size_t total = 0,
                           ingresos = 0,
                           cambios = 0;
                    
                    Glib::ustring detalles;

                    for (auto &&i : j["log"])
                    {
                        detalles += "----------------------------------------\n";
                        detalles += "ID: " + std::to_string(i["id"].get<int>()) + " - ";
                        detalles += i["tipo"].get<std::string>() + "\n";
                        detalles += "Fecha: " +  Glib::DateTime::create_from_iso8601(i["fecha"].get<std::string>()).format("%F %H:%M:%S") + "\n";
                        detalles += "Ingreso: $" + std::to_string(i["ingreso"].get<int>()) + "\n";
                        detalles += "Cambio: $" + std::to_string(i["cambio"].get<int>()) + "\n";
                        detalles += "Total: $" + std::to_string(i["total"].get<int>()) + "\n";
                        detalles += "Estatus: " + i["estatus"].get<std::string>() + "\n";

                        total += i["total"].get<size_t>();
                        ingresos += i["ingreso"].get<size_t>();
                        cambios += i["cambio"].get<size_t>();
                    }

                    detalles += "----------------------------------------\n";
                    auto ti_log = MLog::create(0,Global::User::Current,"Corte de Caja", ingresos, cambios, total, detalles, Glib::DateTime::create_now_local());

                    Global::System::imprime_ticket(ti_log);
                }
            });
        }
        v_dialog->close(); 
    });

    v_dialog->show();
}

void Movimientos::init_datos()
{
    std::vector<Glib::ustring> tipos = {"Todo", "Venta", "Ingreso", "Pago", "Pago Manual", "Refill", "Retirada"};
    m_list_tipos = Gtk::StringList::create(tipos);
    v_dp_tipo->set_model(m_list_tipos);

    auto model_list = Gio::ListStore<MLog>::create();

    // ✅ Sorter por ID (orden descendente por defecto)
    auto uint_expression = Gtk::ClosureExpression<unsigned int>::create(
        [](const Glib::RefPtr<Glib::ObjectBase> &item) -> unsigned int
        {
            const auto col = std::dynamic_pointer_cast<MLog>(item);
            return col ? col->m_id : 0;
        });
    
    m_IdSorter = Gtk::NumericSorter<unsigned int>::create(uint_expression);
    m_IdSorter->set_sort_order(Gtk::SortType::DESCENDING);  

    auto sorter_model = Gtk::SortListModel::create(model_list, m_IdSorter);
    auto selection = Gtk::SingleSelection::create(sorter_model);
    
    v_column_log->set_model(selection);

    {
        auto factory = Gtk::SignalListItemFactory::create();
        factory->signal_setup().connect(sigc::mem_fun(*this, &Movimientos::on_setup_label));
        factory->signal_bind().connect(sigc::mem_fun(*this, &Movimientos::on_bind_id));

        column_id = Gtk::ColumnViewColumn::create("ID", factory);
        column_id->set_sorter(m_IdSorter);
        v_column_log->append_column(column_id);
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
        // column->set_expand();
        v_column_log->append_column(column);
    }

    {
        auto factory = Gtk::SignalListItemFactory::create();
        factory->signal_setup().connect(sigc::mem_fun(*this, &Movimientos::on_setup_label));
        factory->signal_bind().connect(sigc::mem_fun(*this, &Movimientos::on_bind_cambio));
        auto column = Gtk::ColumnViewColumn::create("Cambio", factory);
        // column->set_expand();
        v_column_log->append_column(column);
    }

    {
        auto factory = Gtk::SignalListItemFactory::create();
        factory->signal_setup().connect(sigc::mem_fun(*this, &Movimientos::on_setup_label));
        factory->signal_bind().connect(sigc::mem_fun(*this, &Movimientos::on_bind_total));
        auto column = Gtk::ColumnViewColumn::create("Total", factory);
        // column->set_expand();
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

void Movimientos::reimprime_tickets()
{
    auto row = v_column_log->get_model()->get_selection()->get_minimum();
    if (row == -1)
        return;

    auto single_selection = std::dynamic_pointer_cast<Gtk::SingleSelection>(v_column_log->get_model());
    auto m_list = single_selection->get_typed_object<MLog>(row);

    m_list->m_tipo += " (Reimpresion)";

    Global::System::imprime_ticket(m_list, 0);

    m_list->m_tipo.erase(m_list->m_tipo.size() - 14, 14);
}

void Movimientos::actualiza_data(const Glib::RefPtr<Gtk::SelectionModel> &selection, const Glib::RefPtr<Gio::ListStore<MLog>> &log)
{
    auto single = std::dynamic_pointer_cast<Gtk::SingleSelection>(selection);
    auto sort_store = std::dynamic_pointer_cast<Gtk::SortListModel>(single->get_model());
    auto list_store = std::dynamic_pointer_cast<Gio::ListStore<MLog>>(sort_store->get_model());

    list_store->remove_all();

    for (size_t i = 0; i < log->get_n_items(); i++)
    {
        list_store->append(log->get_item(i));
    }
    //column_id->set_sorter(m_IdSorter);
}

void Movimientos::consume_data()
{
    v_spin_pag->update();

    auto id_tipo = v_dp_tipo->get_selected();
    auto tipo = m_list_tipos->get_string(id_tipo);
    auto f_ini = v_ety_ini->get_text();
    auto f_fin = v_ety_fin->get_text();
    auto pag = v_spin_pag->get_value_as_int();

    pag = pag == 1 ? 0 : pag-1 * 100;

    auto json = nlohmann::json{
        {"tipo", tipo},
        {"f_ini", f_ini},
        {"f_fin", f_fin},
        {"pag", pag}};

    auto future = cpr::PostAsync(cpr::Url{Global::System::URL + "log/movimientos"}, Global::Utility::header, cpr::Body{json.dump()});
    
    Global::Utility::consume_and_do(future, [this](const cpr::Response &response)
    {
        if (response.status_code == 200)
        {
            auto j = nlohmann::json::parse(response.text);

            auto log = std::make_unique<Log>();
            auto m_log = log->get_log(j["log"]);

            auto model = v_column_log->get_model();
            actualiza_data(model, m_log);

            v_lbl_total_registros->set_text(Glib::ustring::format("Mostrando ", m_log->get_n_items(), " de ", j["total_rows"].get<int>()));

            auto pag = j["total_rows"].get<int>() > 0 ? j["total_rows"].get<int>() / m_log->get_n_items() : 0;
            v_ety_pag->set_text("de " + std::to_string(pag));
            v_spin_pag->set_adjustment(Gtk::Adjustment::create(1, 1, pag));
        }
        else
        {
            auto model = v_column_log->get_model();
            actualiza_data(model, Gio::ListStore<MLog>::create());
        }
    });
}

void Movimientos::muestra_calendario_inicio(Gtk::Entry::IconPosition icon_pos)
{
    v_pop_calendario.unparent();
    v_pop_calendario.set_parent(*v_ety_ini);
    v_pop_calendario.popup();
}

void Movimientos::muestra_calendario_fin(Gtk::Entry::IconPosition icon_pos)
{
    v_pop_calendario.unparent();
    v_pop_calendario.set_parent(*v_ety_fin);
    v_pop_calendario.popup();
}

void Movimientos::set_fecha()
{
    if(v_pop_calendario.get_parent() == v_ety_fin)
        ((Gtk::Entry *)v_pop_calendario.get_parent())->set_text(v_calendario.get_date().add_hours(23).add_minutes(59).add_seconds(59).format_iso8601());
    else
    {
        v_ety_fin->set_text(v_calendario.get_date().add_hours(23).add_minutes(59).add_seconds(59).format_iso8601());
        v_ety_ini->set_text(v_calendario.get_date().format_iso8601());
    }
}

void Movimientos::borra_filtro()
{
    v_dp_tipo->set_selected(0);
    v_ety_ini->set_text("");
    v_ety_fin->set_text("");
    v_spin_pag->set_value(1);

    consume_data();
}
