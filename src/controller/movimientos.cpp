#include "controller/movimientos.hpp"

Movimientos::Movimientos(BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &refBuilder) : VMovimientos(cobject, refBuilder)
{
    v_spin_pag->set_adjustment(Gtk::Adjustment::create(1, 1, 100));
    init_datos();

    signal_map().connect(sigc::mem_fun(*this, &Movimientos::init_refrescar_datos));
    v_btn_aplica_filtro->signal_clicked().connect(sigc::mem_fun(*this, &Movimientos::init_refrescar_datos));
    // v_btn_imprime_corte->signal_clicked().connect(sigc::mem_fun(*this, &Movimientos::imprime_corte));
    v_ety_ini->signal_icon_press().connect(sigc::mem_fun(*this, &Movimientos::muestra_calendario_inicio));
    v_ety_fin->signal_icon_press().connect(sigc::mem_fun(*this, &Movimientos::muestra_calendario_fin));
    v_calendario.signal_day_selected().connect(sigc::mem_fun(*this, &Movimientos::set_fecha));
    v_btn_remueve_filtros->signal_clicked().connect(sigc::mem_fun(*this, &Movimientos::borra_filtro));
    // v_btn_printer->signal_clicked().connect(sigc::mem_fun(*this, &Movimientos::reimprime_tickets));
}

Movimientos::~Movimientos()
{
    v_pop_calendario.unparent();
}

void Movimientos::init_refrescar_datos()
{
    // Limpiar cache antes de refrescar
    m_data_cached = false;

    auto root = create_model();
    tree_model_list = Gtk::TreeListModel::create(root, sigc::mem_fun(*this, &Movimientos::create_model));
    auto sorter_model = Gtk::SortListModel::create(tree_model_list, m_IdSorter);
    auto selection = Gtk::MultiSelection::create(sorter_model);
    v_column_log->set_model(selection);
}

void Movimientos::imprime_corte()
{
    auto id_tipo = v_dp_tipo->get_selected();
    auto tipo = m_list_tipos->get_string(id_tipo);
    auto f_ini = v_ety_ini->get_text();
    auto f_fin = v_ety_fin->get_text();

    auto fecha = f_ini.empty() || f_fin.empty() ? "Todo el dia" : "Desde " + Glib::DateTime::create_from_iso8601(f_ini).format("%F") + " hasta " + Glib::DateTime::create_from_iso8601(f_fin).format("%F");

    v_dialog.reset(new Gtk::MessageDialog(*Global::Widget::v_main_window, "Atención", false, Gtk::MessageType::QUESTION, Gtk::ButtonsType::OK_CANCEL, true));
    v_dialog->set_secondary_text("¿Desea imprimir el tipo de movimiento \"" + tipo + "\" y " + fecha + " en el corte de caja?");
    v_dialog->signal_response().connect([this, tipo](int response)
                                        {
        if (Gtk::ResponseType::OK == response)
        {
            auto json = nlohmann::json{{"tipo", tipo},
                                       {"f_ini", v_ety_ini->get_text()},
                                       {"f_fin", v_ety_fin->get_text()}};

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
                        detalles += "Concepto: " + i["descripcion"].get<std::string>() + "\n";
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
                    auto ti_log = MLog::create(0,Global::User::Current,"Corte de Caja", "", std::to_string(ingresos), std::to_string(cambios), total, detalles, Glib::DateTime::create_now_local());

                    Global::System::imprime_ticket(ti_log);
                }
            });
        }
        v_dialog->close(); });

    v_dialog->show();
}

void Movimientos::init_datos()
{
    std::vector<Glib::ustring> tipos = {"Todo", "Venta", "Ingreso", "Pago", "Pago Manual", "Refill", "Transpaso", "Retirada de Casette"};
    m_list_tipos = Gtk::StringList::create(tipos);
    v_dp_tipo->set_model(m_list_tipos);

    // ✅ Sorter por ID (orden descendente por defecto)
    auto uint_expression = Gtk::ClosureExpression<unsigned int>::create(
        [](const Glib::RefPtr<Glib::ObjectBase> &item) -> unsigned int
        {
            const auto col = std::dynamic_pointer_cast<MLog>(item);
            return col ? col->m_id : 0;
        });

    m_IdSorter = Gtk::NumericSorter<unsigned int>::create(uint_expression);
    m_IdSorter->set_sort_order(Gtk::SortType::DESCENDING);

    auto sorter_model = Gtk::SortListModel::create(tree_model_list, m_IdSorter);
    auto selection = Gtk::MultiSelection::create(sorter_model);

    v_column_log->set_model(selection);

    {
        auto factory = Gtk::SignalListItemFactory::create();
        factory->signal_setup().connect(sigc::mem_fun(*this, &Movimientos::on_setup_id));
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
        factory->signal_bind().connect(sigc::mem_fun(*this, &Movimientos::on_bind_concepto));
        auto column = Gtk::ColumnViewColumn::create("Concepto", factory);
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
        auto column = Gtk::ColumnViewColumn::create("Salidas", factory);
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

    auto click_gesture = Gtk::GestureClick::create();
    click_gesture->set_button(GDK_BUTTON_SECONDARY); // Solo clic derecho
    click_gesture->signal_pressed().connect( sigc::mem_fun(*this, &Movimientos::on_column_view_button_pressed), false);

    // // También queremos clic izquierdo para selección normal
    // auto left_click = Gtk::GestureClick::create();
    // left_click->set_button(GDK_BUTTON_PRIMARY);
    // left_click->signal_pressed().connect([this](int n_press, double x, double y)
    //                                      {
    //                                          // El comportamiento por defecto ya maneja la selección
    //                                      });

    v_column_log->add_controller(click_gesture);
    // v_column_log->add_controller(left_click);

    // Crear el menú popup
    create_context_menu();
}

void Movimientos::create_context_menu()
{
    // Crear el modelo del menú
    auto menu_model = Gio::Menu::create();

    // Agregar items
    m_menu_item_reimprimir = Gio::MenuItem::create("Reimprimir ticket/s", "win.reimprimir");
    m_menu_item_detalles = Gio::MenuItem::create("Imprimir Corte", "win.imprime_corte");
    m_menu_item_anular = Gio::MenuItem::create("Borrar Filtros", "win.borra_filtro");

    menu_model->append_item(m_menu_item_reimprimir);
    menu_model->append_item(m_menu_item_detalles);
    menu_model->append_item(m_menu_item_anular);

    // Crear el popover con el modelo
    m_popover_menu = Gtk::manage(new Gtk::PopoverMenu);
    m_popover_menu->set_menu_model(menu_model);
    m_popover_menu->set_parent(*v_column_log);

    // Configurar acciones de la ventana
    auto action_group = Gio::SimpleActionGroup::create();

    action_group->add_action("reimprimir", sigc::mem_fun(*this, &Movimientos::reimprime_tickets));
    action_group->add_action("imprime_corte", sigc::mem_fun(*this, &Movimientos::imprime_corte));
    action_group->add_action("borra_filtro", sigc::mem_fun(*this, &Movimientos::borra_filtro));

    insert_action_group("win", action_group);
}

Glib::RefPtr<Gio::ListModel> Movimientos::create_model(const Glib::RefPtr<Glib::ObjectBase> &log)
{
    // Usar datos cacheados
    const auto &data = consume_data();
    auto col = std::dynamic_pointer_cast<MLog>(log);
    auto result = Gio::ListStore<MLog>::create();

    for (auto &&i : data["log"])
    {
        if (!col) // Nivel raíz - agregar movimientos principales
        {
            result->append(MLog::create(
                i["id"].get<int>(),
                i["usuario"].get<std::string>(),
                i["tipo"].get<std::string>(),
                i["descripcion"].get<std::string>(),
                std::to_string(i["ingreso"].get<int>()),
                std::to_string(i["cambio"].get<int>()),
                i["total"].get<int>(),
                i["estatus"].get<std::string>(),
                Glib::DateTime::create_from_iso8601(i["fecha"].get<std::string>())));
        }
        else // Nivel hijo - agregar detalles del movimiento actual
        {
            if (i.contains("detalle_movimiento") && i["id"].get<int>() == col->m_id)
            {
                for (auto &&det : i["detalle_movimiento"])
                {
                    result->append(MLog::create(
                        0, // ID 0 para hijos
                        "",
                        det["tipo_movimiento"].get<std::string>(), // Usar tipo_movimiento
                        "",
                        (det["tipo_movimiento"].get<std::string>() == "entrada") ? std::to_string(det["denominacion"].get<int>()) + " → " + std::to_string(det["cantidad"].get<int>()) : "0",
                        (det["tipo_movimiento"].get<std::string>() == "salida") ? std::to_string(det["denominacion"].get<int>()) + " ← " + std::to_string(det["cantidad"].get<int>()) : "0",
                        det["denominacion"].get<int>() * det["cantidad"].get<int>(),
                        "",
                        Glib::DateTime::create_from_iso8601(det["creado_en"].get<std::string>())));
                }
                // break;  // Importante: salir del loop cuando encontramos el movimiento
            }
        }
    }

    return (result->get_n_items() > 0) ? result : Glib::RefPtr<Gio::ListModel>();
}

void Movimientos::on_column_view_button_pressed(int n_press, double x, double y)
{
    m_popover_menu->set_pointing_to(Gdk::Rectangle{(int)x, (int)y, 1, 1});
    m_popover_menu->popup();
}

void Movimientos::reimprime_tickets()
{
    auto selection = std::dynamic_pointer_cast<Gtk::MultiSelection>(
        v_column_log->get_model());
    if (!selection)
        return;

    auto bitset = selection->get_selection();
    auto sorter_model = std::dynamic_pointer_cast<Gtk::SortListModel>(
        selection->get_model());
    if (!sorter_model)
        return;

    // Range-based for con iteradores de Bitset
    for (guint pos : *bitset)
    { 
        auto row = std::dynamic_pointer_cast<Gtk::TreeListRow>(sorter_model->get_object(pos));
        if (!row)
            continue;

        auto mlog = std::dynamic_pointer_cast<MLog>(row->get_item());
        if (mlog && mlog->m_id != 0)
        {
            mlog->m_tipo += " (Reimpreso)";
            Global::System::imprime_ticket(mlog, 0);
            mlog->m_tipo.erase(mlog->m_tipo.size() - 14, 14); // Eliminar " (Reimpreso)" del tipo para futuras impresiones
        }
    }
}

const nlohmann::json &Movimientos::consume_data()
{
    // Si ya tenemos datos cacheados, devolverlos
    if (m_data_cached)
        return m_cached_data;

    // Si no, hacer la petición HTTP
    v_spin_pag->update();

    auto id_tipo = v_dp_tipo->get_selected();
    auto tipo = m_list_tipos->get_string(id_tipo);
    auto f_ini = v_ety_ini->get_text();
    auto f_fin = v_ety_fin->get_text();
    auto pag = v_spin_pag->get_value_as_int();

    pag = pag == 1 ? 0 : pag - 1 * 100;

    auto json = nlohmann::json{
        {"tipo", tipo},
        {"f_ini", f_ini},
        {"f_fin", f_fin},
        {"pag", pag}};

    auto future = cpr::PostAsync(cpr::Url{Global::System::URL + "log/movimientos"},
                                 Global::Utility::header,
                                 cpr::Body{json.dump()});

    auto response = future.get();

    if (response.status_code == 200)
    {
        m_cached_data = nlohmann::json::parse(response.text);
        m_data_cached = true;

        // Actualizar UI
        v_lbl_total_registros->set_text(Glib::ustring::format(
            "Mostrando ", m_cached_data["log"].size(),
            " de ", m_cached_data["total_rows"].get<int>()));

        auto pag = m_cached_data["total_rows"].get<int>() > 0
                       ? m_cached_data["total_rows"].get<int>() / m_cached_data["log"].size()
                       : 0;
        v_ety_pag->set_text("de " + std::to_string(pag));
        v_spin_pag->set_adjustment(Gtk::Adjustment::create(1, 1, pag));
    }
    else
    {
        m_cached_data = nlohmann::json();
        m_data_cached = true; // Cachear incluso el resultado vacío

        v_lbl_total_registros->set_text("No se pudieron cargar los movimientos.");
        v_ety_pag->set_text("");
        v_spin_pag->set_adjustment(Gtk::Adjustment::create(1, 1, 1));
    }

    return m_cached_data;
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
    if (v_pop_calendario.get_parent() == v_ety_fin)
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
