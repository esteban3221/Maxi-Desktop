#include "controller/configuracion/impresora.hpp"

Impresora::Impresora(BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &refBuilder) : VImpresora(cobject, refBuilder),
                                                                                              text_buffer(Gtk::TextBuffer::create())
{
#ifdef __linux__
    signal_map().connect(sigc::mem_fun(*this, &Impresora::init_impresoras_linux));
#elif defined(_WIN32) || defined(_WIN64)
    signal_map().connect(sigc::mem_fun(*this, &Impresora::init_impresoras_windows));
#endif

    m_refPageSetup = Gtk::PageSetup::create();
    m_refSettings = Gtk::PrintSettings::create();

    v_text_view->set_buffer(text_buffer);
    test_text_impresion(0);
    signal_map().connect(sigc::mem_fun(*this, &Impresora::init_local));
    signal_map().connect(sigc::mem_fun(*this, &Impresora::init_remoto));

    v_ntbook_propiedades->signal_switch_page().connect(sigc::mem_fun(*this, &Impresora::on_switch_tab_changed));

    local.v_lbox_activa->signal_row_activated().connect(sigc::mem_fun(*this, &Impresora::on_activalist_activate));
    local.v_lbox_vizualizador->signal_row_activated().connect(sigc::mem_fun(*this, &Impresora::on_vizaliza_list_activate));
    local.v_lbox_test->signal_row_activated().connect(sigc::mem_fun(*this, &Impresora::on_list_test_printer));

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
                                   Global::Utility::header);

    Global::Utility::consume_and_do(response, [this](const cpr::Response &response)
                                    {
        
        if (response.status_code == 200)
            Global::Widget::reveal_toast("Configuracion de impresion guardada correctamente");
        else
            Global::Widget::reveal_toast("Error al guardar la configuracion de impresion", (Gtk::MessageType)3); });
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
}

void Impresora::on_vizaliza_list_activate_remoto(Gtk::ListBoxRow *row)
{
}

void Impresora::on_list_test_printer(Gtk::ListBoxRow *row)
{
    m_refPrintFormOperation = PrintFormOperation::create();

    m_refPrintFormOperation->set_markup(ticket_markup);

    m_refPrintFormOperation->set_track_print_status();
    m_refSettings->set_paper_height(80, Gtk::Unit::MM);
    m_refSettings->set_paper_width(297, Gtk::Unit::MM);

    m_refPrintFormOperation->set_default_page_setup(m_refPageSetup);
    m_refPrintFormOperation->set_print_settings(m_refSettings);

    try
    {
        m_refPrintFormOperation->run(Gtk::PrintOperation::Action::PRINT);
        m_refPrintFormOperation->signal_done().connect([this](Gtk::PrintOperation::Result result)
                                                       {
            
            Global::Widget::reveal_toast("Impresion realizada correctamente"); });
    }
    catch (const Gtk::PrintError &ex)
    {
        // See documentation for exact Gtk::PrintError error codes.
        std::cerr << "An error occurred while trying to run a print operation:"
                  << ex.what() << std::endl;
    }
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
                {
                    printer->v_image_check->set_opacity(1);
                    m_refSettings->set_printer(impresora_default);
                }
                    
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
    auto response = cpr::GetAsync(cpr::Url{Global::System::URL, "configuracion/get_informacion_impresora"},Global::Utility::header);
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
        } 
    });
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
    auto response = cpr::GetAsync(cpr::Url{Global::System::URL, "configuracion/get_informacion_empresa"},Global::Utility::header);

    Global::Utility::consume_and_do(response, [this, id](const cpr::Response &response)
                                    {
        if (response.status_code == 200)
        {
            text_buffer->set_text("");

            auto json = nlohmann::json::parse(response.text);
            auto ticket_config = std::make_unique<std::stringstream>();

            *ticket_config << "****** <span weight=\"bold\">TICKET DE COMPRA</span> ******\n"
                        << "--------------------------------\n\n"
                        << json["razon_social"].get<std::string>() << "\n\n";
            

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
            *ticket_config << "<span weight=\"bold\">Total</span>: \t60.00\n";
            *ticket_config << "<span weight=\"bold\">Tipo de Pago</span>:\tEfectivo\n"
                        << "--------------------------------\n";
            *ticket_config << "<span weight=\"bold\">Ingreso</span>:\t90.00\n";
            *ticket_config << "<span weight=\"bold\">Cambio</span>:\t30.00\n"
                        << "--------------------------------\n";

            if ((id ? remoto : local ).v_check_vizualizador[5]->get_active())
                *ticket_config << "**<span weight=\"bold\">" << json["contacto"].get<std::string>() << "</span>**"
                            << "\n"
                            << "--------------------------------\n";
            if ((id ? remoto : local ).v_check_vizualizador[0]->get_active())
                *ticket_config << "**<span weight=\"bold\">" << json["agradecimiento"].get<std::string>()<< "</span>**"
                            << "\n"
                            << "--------------------------------\n";

            ticket_markup = ticket_config->str();
            text_buffer->insert_markup(text_buffer->end(), ticket_config->str());
        } });
}

namespace Global
{
    namespace System
    {
        void imprime_ticket(Glib::RefPtr<MLog> log, int faltante)
        {

            auto db = std::make_unique<Configuracion>();
            auto db_ = db->get_conf_data(5, 11);

            if (db_->get_item(0)->m_valor != "1")
                return;

            std::stringstream ticket_config;

            auto response = cpr::Get(cpr::Url{Global::System::URL, "configuracion/get_informacion_empresa"}, Global::Utility::header);
            if (response.status_code != 200)
                return;
            else
            {
                auto json = nlohmann::json::parse(response.text);

                ticket_config << "****** <span weight=\"bold\">" << log->m_tipo << "</span> ******\n"
                              << "--------------------------------\n\n"
                              << json["razon_social"].get<std::string>() << "\n\n";

                if (db_->get_item(3)->m_valor == "1")
                    ticket_config << "Direccion:<span weight=\"bold\">" << json["direccion"].get<std::string>() << "</span>\n"
                                  << "--------------------------------\n";

                if (db_->get_item(4)->m_valor == "1")
                    ticket_config << "RFC:<span weight=\"bold\">" << json["rfc"].get<std::string>() << "</span>\n"
                                  << "--------------------------------\n";

                if (db_->get_item(2)->m_valor == "1")
                    ticket_config << "Fecha:<span weight=\"bold\">" << Glib::DateTime::create_now_local().format("%Y-%m-%d %H:%M:%S") << "</span>\n";

                ticket_config << "ID:<span weight=\"bold\">" << log->m_id << "</span>\n\n";

                if (db_->get_item(5)->m_valor == "1")
                    ticket_config << "Le atendio: " << log->m_user << "\n\n"
                                  << "--------------------------------\n";

                ticket_config << "Total:\t<span weight=\"bold\">" << log->m_total << "</span>\n";
                ticket_config << "Tipo de Pago:\t" << "Efectivo\n"
                              << "--------------------------------\n";
                ticket_config << "Ingreso:\t" << log->m_ingreso << "\n";
                ticket_config << "Cambio:\t" << log->m_cambio << "\n";
                ticket_config << "Faltante:\t" << faltante << "\n";

                ticket_config << "-------------STATUS------------\n"
                              << "<span weight=\"bold\">" << log->m_estatus << "</span>\n"
                              << "--------------------------------\n";

                if (db_->get_item(6)->m_valor == "1")
                    ticket_config << "**<span weight=\"bold\">" << json["contacto"].get<std::string>() << "</span>**\n"
                                  << "--------------------------------\n";

                if (db_->get_item(1)->m_valor == "1")
                    ticket_config << "**<span weight=\"bold\">" << json["agradecimiento"].get<std::string>() << "</span>**\n"
                                  << "--------------------------------\n";
                
                auto m_refPrintFormOperation = PrintFormOperation::create();
                auto m_refSettings = Gtk::PrintSettings::create();
                auto m_refPageSetup = Gtk::PageSetup::create();

                m_refPrintFormOperation->set_markup(ticket_config.str());
                              
                m_refPrintFormOperation->set_track_print_status();

                auto p_size = m_refSettings->get_paper_size();
                p_size.set_size(297, 80, Gtk::Unit::MM);
                              
                m_refPrintFormOperation->set_default_page_setup(m_refPageSetup);
                m_refPrintFormOperation->set_print_settings(m_refSettings);
                              
                try
                {
                    m_refPrintFormOperation->run(Gtk::PrintOperation::Action::PRINT, *Global::Widget::v_main_window);
                    m_refPrintFormOperation->signal_done().connect([](Gtk::PrintOperation::Result result)
                    {
                        Global::Widget::reveal_toast("Impresion realizada correctamente");
                    });
                }
                catch (const Gtk::PrintError &ex)
                {
                    Global::Widget::reveal_toast(ex.what(), (Gtk::MessageType)3);
                    std::cerr << "An error occurred while trying to run a print operation:"
                              << ex.what() << std::endl;
                }
                
            }
        }
    }
}