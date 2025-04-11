#include "controller/configuracion/impresora.hpp"

Impresora::Impresora(BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &refBuilder) : VImpresora(cobject, refBuilder),
                                                                                              text_buffer(Gtk::TextBuffer::create())
{
#ifdef __linux__
    signal_map().connect(sigc::mem_fun(*this, &Impresora::init_impresoras_linux));
#elif defined(_WIN32) || defined(_WIN64)
    signal_map().connect(sigc::mem_fun(*this, &Impresora::init_impresoras_windows));
#endif

    v_text_view->set_buffer(text_buffer);
    test_text_impresion(0);
    signal_map().connect(sigc::mem_fun(*this, &Impresora::init_local));
    signal_map().connect(sigc::mem_fun(*this, &Impresora::init_remoto));

    v_ntbook_propiedades->signal_switch_page().connect(sigc::mem_fun(*this, &Impresora::on_switch_tab_changed));

    local.v_lbox_activa->signal_row_activated().connect(sigc::mem_fun(*this, &Impresora::on_activalist_activate));
    local.v_lbox_vizualizador->signal_row_activated().connect(sigc::mem_fun(*this, &Impresora::on_vizaliza_list_activate));

    v_list_box_print->signal_row_activated().connect(sigc::mem_fun(*this, &Impresora::on_list_box_row_selected));
    remoto.v_lbox_test->signal_row_activated().connect(sigc::mem_fun(*this, &Impresora::on_list_remoto_guardar));
}

Impresora::~Impresora()
{
}

void Impresora::on_list_remoto_guardar(Gtk::ListBoxRow *)
{
    auto json = nlohmann::json{
        {"activa", remoto.v_switch_activa->get_active()},
        {"agradecimiento", remoto.v_check_vizualizador[0]->get_active()},
        {"fecha", remoto.v_check_vizualizador[1]->get_active()},
        {"direccion", remoto.v_check_vizualizador[2]->get_active()},
        {"rfc", remoto.v_check_vizualizador[3]->get_active()},
        {"vendedor", remoto.v_check_vizualizador[4]->get_active()},
        {"contacto", remoto.v_check_vizualizador[5]->get_active()}};

    auto response = cpr::PostAsync(cpr::Url{Global::System::URL, "configuracion/actualiza_impresion"},
                                   cpr::Body{json.dump()},
                                   cpr::Header{{"Content-Type", "application/json"}});

    Global::Utility::consume_and_do(response, [this](const cpr::Response &response)
                                    {
        Global::Widget::v_revealer->set_reveal_child();
        if (response.status_code == 200)
            Global::Widget::v_revealer_title->set_text("Configuracion de impresion guardada correctamente");
        else
            Global::Widget::v_revealer_title->set_text("Error al guardar la configuracion de impresion"); });
}

void Impresora::on_activalist_activate(Gtk::ListBoxRow *row)
{
    auto db_impresora = std::make_unique<Configuracion>();

    local.v_switch_activa->set_active(!local.v_switch_activa->get_active());
    auto estado = local.v_switch_activa->get_active() ? "1" : "0";
    db_impresora->update_conf(MConfiguracion::create(5, "Impresion local", estado));
}

void Impresora::on_vizaliza_list_activate(Gtk::ListBoxRow *row)
{
    auto db = std::make_unique<Configuracion>();
    auto indice = row->get_index();
    local.v_check_vizualizador[indice]->set_active(!local.v_check_vizualizador[indice]->get_active());
    test_text_impresion(0);
    db->update_conf(MConfiguracion::create(6 + indice,
                                           "Visualiza Impresion",
                                           std::to_string(local.v_check_vizualizador[indice]->get_active())));

    // actualiza_buffer();
}

void Impresora::on_vizaliza_list_activate_remoto(Gtk::ListBoxRow *row)
{
    // auto db = std::make_unique<Configuracion>();
    // remoto.v_switch_activa->set_active(!remoto.v_switch_activa->get_active());
    // db->update_conf(MConfiguracion::create(5,
    //                                        "Impresion Local",
    //                                        std::to_string(remoto.v_switch_activa->get_active())));

    // actualiza_buffer();
}

void Impresora::on_switch_tab_changed(Gtk::Widget *, guint id)
{
    if (id == 0)
    {
        init_local();
    }
    else if (id == 1)
    {
        init_remoto();
    }
    test_text_impresion(id);
}

#ifdef __linux__
void Impresora::init_impresoras_linux()
{
    std::thread([this]()
                {
        cups_dest_t *printers;
        int num_printers = cupsGetDests(&printers);

        if (num_printers > 0)
        {
            v_list_box_print->remove_all();
            auto db_impresora = std::make_unique<Configuracion>();
            auto db_contendor = db_impresora->get_conf_data(4, 4);
            std::string impresora_default = db_contendor->get_item(0)->m_valor;

            for (int i = 0; i < num_printers; i++)
            {
                std::string name = printers[i].name;
                std::string subtitle = cupsGetOption("device-uri", printers[i].num_options, printers[i].options);
                if (subtitle.empty())
                    subtitle = "None";
                auto printer = Gtk::make_managed<VListPrinters>(name, subtitle);
                if (name == impresora_default)
                    printer->v_image_check->set_opacity(1);

                v_list_box_print->append(*printer);
            }
        }

        cupsFreeDests(num_printers, printers); })
        .detach();
}
#endif

#ifdef _WIN32
std::vector<Impresora::PrinterInfo> Impresora::listarImpresoras()
{
    std::vector<Impresora::PrinterInfo> printers;
    DWORD needed = 0, returned = 0;

    // Primera llamada para obtener el tamaño del buffer
    EnumPrintersW(
        PRINTER_ENUM_LOCAL | PRINTER_ENUM_CONNECTIONS,
        nullptr,
        2, // PRINTER_INFO_2 incluye ubicación
        nullptr,
        0,
        &needed,
        &returned);

    if (needed == 0)
    {
        std::wcerr << L"Error al calcular el tamaño: " << GetLastError() << std::endl;
        return printers;
    }

    // Reservar memoria
    std::vector<BYTE> buffer(needed);

    // Segunda llamada para obtener datos
    if (!EnumPrintersW(
            PRINTER_ENUM_LOCAL | PRINTER_ENUM_CONNECTIONS,
            nullptr,
            2,
            buffer.data(),
            needed,
            &needed,
            &returned))
    {
        std::wcerr << L"Error al enumerar impresoras: " << GetLastError() << std::endl;
        return printers;
    }

    // Procesar resultados
    PRINTER_INFO_2W *printerInfo = reinterpret_cast<PRINTER_INFO_2W *>(buffer.data());
    for (DWORD i = 0; i < returned; ++i)
    {
        PrinterInfo info;
        info.name = Global::Utility::WStrToUTF8(printerInfo[i].pPrinterName);

        if (printerInfo[i].pPortName)
        {
            info.uri = Global::Utility::WStrToUTF8(printerInfo[i].pPortName);
        }

        printers.push_back(info);
    }

    return printers;
};


void Impresora::init_impresoras_windows()
{
    v_list_box_print->remove_all();
    auto db_impresora = std::make_unique<Configuracion>();
    auto db_contendor = db_impresora->get_conf_data(4, 4);
    std::string impresora_default = db_contendor->get_item(0)->m_valor;
    
    for (auto &&i : listarImpresoras())
    {
        auto printer = Gtk::make_managed<VListPrinters>(i.name, i.uri);
        v_list_box_print->append(*printer);
        if (i.name == impresora_default)
            printer->v_image_check->set_opacity(1);
    }
    
}
#endif

void Impresora::init_local()
{
    auto db_impresora = std::make_unique<Configuracion>();
    auto db_contendor = db_impresora->get_conf_data(5, 11);

    local.v_switch_activa->set_active(db_contendor->get_item(0)->m_valor == "1");

    for (size_t i = 0; i < 6; i++)
        local.v_check_vizualizador[i]->set_active(db_contendor->get_item(i + 1)->m_valor == "1");
}

void Impresora::init_remoto()
{
    auto response = cpr::GetAsync(cpr::Url{Global::System::URL, "configuracion/get_informacion_impresora"});
    Global::Utility::consume_and_do(response, [this](const cpr::Response &response)
                                    {
        if (response.status_code == 200)
        {
            auto json = nlohmann::json::parse(response.text);
        
            remoto.v_switch_activa->set_active(json["activa"].get<bool>());

            remoto.v_check_vizualizador[0]->set_active(json["agradecimiento"].get<bool>());
            remoto.v_check_vizualizador[1]->set_active(json["fecha"].get<bool>());
            remoto.v_check_vizualizador[2]->set_active(json["direccion"].get<bool>());
            remoto.v_check_vizualizador[3]->set_active(json["rfc"].get<bool>());
            remoto.v_check_vizualizador[4]->set_active(json["vendedor"].get<bool>());
            remoto.v_check_vizualizador[5]->set_active(json["contacto"].get<bool>());
        } });
}

void Impresora::on_list_box_row_selected(Gtk::ListBoxRow *row)
{
    auto this_row = dynamic_cast<VImpresora::VListPrinters *>(row);
    auto db_impresora = std::make_unique<Configuracion>();
    for (auto &&i : v_list_box_print->get_children())
    {
        auto printer = dynamic_cast<VListPrinters *>(i);
        if (printer)
        {
            printer->v_image_check->set_opacity(0);
            if (printer == this_row)
            {
                printer->v_image_check->set_opacity(1);
                auto id_impresora = this_row->v_titulo->get_text();
                db_impresora->update_conf(MConfiguracion::create(4, "Impresora default", id_impresora));
            }
        }
    }
}

void Impresora::test_text_impresion(int id)
{
    auto response = cpr::GetAsync(cpr::Url{Global::System::URL, "configuracion/get_informacion_empresa"});
    Global::Utility::consume_and_do(response, [this, id](const cpr::Response &response)
                                    {
        if (response.status_code == 200)
        {
            text_buffer->set_text("");
            auto json = nlohmann::json::parse(response.text);
            auto ticket_config = std::make_unique<std::stringstream>();

            *ticket_config << "****** <span weight=\"bold\">TICKET DE COMPRA</span> ******\n"
                        << "--------------------------------\n\n"
                        << std::left << std::setw(20) << json["razon_social"].get<std::string>() << "\n\n";
            

            if ((id ? remoto : local ).v_check_vizualizador[2]->get_active())
                *ticket_config << "<span weight=\"bold\">Direccion</span>: " << json["direccion"].get<std::string>() << "\n"
                            << "--------------------------------\n";

            if ((id ? remoto : local ).v_check_vizualizador[3]->get_active())
                *ticket_config << "<span weight=\"bold\">RFC</span>: " << json["rfc"].get<std::string>()<< "\n"
                            << "--------------------------------\n";

            if ((id ? remoto : local ).v_check_vizualizador[1]->get_active())
                *ticket_config << "<span weight=\"bold\">Fecha</span>: "
                            << Glib::DateTime::create_now_local().format("%Y-%m-%d %H:%M:%S") << "\n";

            *ticket_config << "No. Ticket: 0001"
                        << "\n\n";

            if ((id ? remoto : local ).v_check_vizualizador[4]->get_active())
                *ticket_config << std::left << std::setw(10) << "<span weight=\"bold\">Le atendio</span>: "
                            << "Juan Perez \n\n"
                            << "--------------------------------\n";

            *ticket_config << "<span weight=\"bold\">Articulo\n"
                        << std::left << std::setw(10) << "Cnt." << std::setw(10) << "P.U."
                        << "T.</span>\n"
                        << "--------------------------------\n";
            *ticket_config << "<span weight=\"bold\">Articulo de prueba</span>\n"
                        << std::setw(10) << "10" << std::setw(10) << "6"
                        << std::setw(10) << "60"
                        << "\n"
                        << "--------------------------------\n";
            *ticket_config << std::left << std::setw(20) << "<span weight=\"bold\">Total</span>:"
                        << "60.00\n";
            *ticket_config << std::left << std::setw(20) << "<span weight=\"bold\">Tipo de Pago</span>:"
                        << "Efectivo\n"
                        << "--------------------------------\n";
            *ticket_config << std::left << std::setw(20) << "<span weight=\"bold\">Ingreso</span>:"
                        << "90.00\n";
            *ticket_config << std::left << std::setw(20) << "<span weight=\"bold\">Cambio</span>:"
                        << "30.00\n"
                        << "--------------------------------\n";

            if ((id ? remoto : local ).v_check_vizualizador[5]->get_active())
                *ticket_config << "**<span weight=\"bold\">" << json["contacto"].get<std::string>() << "</span>**"
                            << "\n"
                            << "--------------------------------\n";
            if ((id ? remoto : local ).v_check_vizualizador[0]->get_active())
                *ticket_config << "**<span weight=\"bold\">" << json["agradecimiento"].get<std::string>()<< "</span>**"
                            << "\n"
                            << "--------------------------------\n";

            
            text_buffer->insert_markup(text_buffer->end(), ticket_config->str());
        } });
}

namespace Global
{
    namespace System
    {
        std::string imprime_ticket(Glib::RefPtr<MLog> log, int faltante)
        {
            std::stringstream ticket_config;
            auto db = std::make_unique<Configuracion>();
            auto db_empresa = db->get_conf_data(10, 14);

            ticket_config << "****** " << log->m_tipo << " ******\n"
                          << "--------------------------------\n\n"
                          << std::left << std::setw(20) << db_empresa->get_item(0)->m_valor << "\n\n";

            if (Global::Widget::Impresora::state_vizualizacion[0])
                ticket_config << "Direccion: " << db_empresa->get_item(1)->m_valor << "\n"
                              << "--------------------------------\n";

            if (Global::Widget::Impresora::state_vizualizacion[1])
                ticket_config << "RFC: " << db_empresa->get_item(2)->m_valor << "\n"
                              << "--------------------------------\n";

            if (Global::Widget::Impresora::state_vizualizacion[2])
                ticket_config << "Fecha: " << Glib::DateTime::create_now_local().format("%Y-%m-%d %H:%M:%S") << "\n";

            ticket_config << "No. Ticket: " << log->m_id << "\n\n";

            if (Global::Widget::Impresora::state_vizualizacion[3])
                ticket_config << std::left << std::setw(10) << "Le atendio: " << Global::User::Current << "\n\n"
                              << "--------------------------------\n";

            ticket_config << std::left << std::setw(20) << "Total:" << log->m_total << "\n";
            ticket_config << std::left << std::setw(20) << "Tipo de Pago:" << "Efectivo\n"
                          << "--------------------------------\n";
            ticket_config << std::left << std::setw(20) << "Ingreso:" << log->m_ingreso << "\n";
            ticket_config << std::left << std::setw(20) << "Cambio:" << log->m_cambio << "\n";
            ticket_config << std::left << std::setw(20) << "Faltante:" << faltante << "\n";
            ticket_config << std::left << std::setw(20) << "Faltante Cambio:" << 0 << "\n"
                          << "-------------STATUS------------\n"
                          << log->m_estatus << "\n"
                          << "--------------------------------\n";

            if (Global::Widget::Impresora::state_vizualizacion[4])
                ticket_config << "**" << db_empresa->get_item(3)->m_valor << "**\n"
                              << "--------------------------------\n";

            if (Global::Widget::Impresora::state_vizualizacion[5])
                ticket_config << "**" << db_empresa->get_item(4)->m_valor << "**\n"
                              << "--------------------------------\n";

            return ticket_config.str();
        }
    }
}