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
                                                       { Global::Widget::reveal_toast("Impresion realizada correctamente"); });
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
    auto response = cpr::GetAsync(cpr::Url{Global::System::URL, "configuracion/get_informacion_impresora"}, Global::Utility::header);
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
    auto response = cpr::GetAsync(cpr::Url{Global::System::URL + "configuracion/get_informacion_empresa"},
                                  Global::Utility::header);

    Global::Utility::consume_and_do(response, [this, id](const cpr::Response& response)
    {
        if (response.status_code != 200)
            return;

        text_buffer->set_text("");  // Limpiar buffer

        auto json = nlohmann::json::parse(response.text);

        std::stringstream ticket;

        // Encabezado centrado y destacado
        ticket << "<span size=\"x-large\" weight=\"bold\">TICKET DE COMPRA</span>\n"
               << "<span size=\"small\">" << Glib::DateTime::create_now_local().format("%Y-%m-%d %H:%M:%S") << "</span>\n"
               << "====================================\n\n";

        // Datos de la empresa (opcionales)
        ticket << "<span weight=\"bold\">" << json["razon_social"].get<std::string>() << "</span>\n";

        if ((id ? remoto : local).v_check_vizualizador[2]->get_active())
            ticket << json["direccion"].get<std::string>() << "\n";

        if ((id ? remoto : local).v_check_vizualizador[3]->get_active())
            ticket << "RFC: " << json["rfc"].get<std::string>() << "\n";

        ticket << "------------------------------------\n";

        // Información general
        ticket << "<span weight=\"bold\">No. Ticket:</span> 0001\n\n";

        if ((id ? remoto : local).v_check_vizualizador[4]->get_active())
            ticket << "<span weight=\"bold\">Le atendió:</span> Juan Pérez\n"
                   << "------------------------------------\n";

        // Detalle de artículos (ejemplo fijo)
        ticket << "<span weight=\"bold\">Artículo\t\tCnt.\tP.U.\tT.</span>\n"
               << "------------------------------------\n"
               << "<span weight=\"bold\">Artículo de prueba</span>\n"
               << std::setw(24) << "10\t\t6\t\t60\n"
               << "------------------------------------\n";

        // Montos alineados
        ticket << std::left << std::setw(18) << "<span weight=\"bold\">Total:</span>"
               << std::right << std::setw(18) << "<span weight=\"bold\" size=\"large\">$60.00</span>\n";

        ticket << std::left << std::setw(18) << "<span weight=\"bold\">Tipo de pago:</span>"
               << std::right << std::setw(18) << "Efectivo\n";

        ticket << std::left << std::setw(18) << "<span weight=\"bold\">Ingreso:</span>"
               << std::right << std::setw(18) << "$90.00\n";

        ticket << std::left << std::setw(18) << "<span weight=\"bold\">Cambio:</span>"
               << std::right << std::setw(18) << "$30.00\n";

        ticket << "------------------------------------\n";

        // Pie de página
        if ((id ? remoto : local).v_check_vizualizador[5]->get_active())
            ticket << "<span size=\"small\" style=\"italic\">"
                   << json["contacto"].get<std::string>() << "</span>\n";

        if ((id ? remoto : local).v_check_vizualizador[0]->get_active())
            ticket << "\n<span size=\"small\" style=\"italic\" weight=\"bold\">"
                   << json["agradecimiento"].get<std::string>() << "</span>\n";

        ticket << "\n\n\n";  // Espacio para que se vea bien en el buffer

        // Insertar con markup en el TextBuffer
        ticket_markup = ticket.str();
        text_buffer->insert_markup(text_buffer->end(), ticket.str());
    });
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

            // Obtener datos de la empresa
            auto response = cpr::Get(cpr::Url{Global::System::URL + "configuracion/get_informacion_empresa"},
                                     Global::Utility::header);

            if (response.status_code != 200)
                return;

            auto json = nlohmann::json::parse(response.text);

            std::stringstream ticket;

            // Encabezado centrado y destacado
            ticket << "<span size=\"x-large\" weight=\"bold\">" << log->m_tipo << "</span>\n"
            //Dexcriocion¿ / concepto de la operacion
                   << std::left << std::setw(20) << "<span weight=\"bold\">Descripcion:</span>"
                   << std::right << std::setw(20) << log->m_descripcion << "\n"
                   << "<span size=\"small\">" << Glib::DateTime::create_now_local().format("%Y-%m-%d %H:%M:%S") << "</span>\n"
                   << "====================================\n\n";

            // Datos de la empresa (opcionales según configuración)
            ticket << "<span weight=\"bold\">" << json["razon_social"].get<std::string>() << "</span>\n";

            if (db_->get_item(3)->m_valor == "1")
                ticket << json["direccion"].get<std::string>() << "\n";

            if (db_->get_item(4)->m_valor == "1")
                ticket << "RFC: " << json["rfc"].get<std::string>() << "\n";

            ticket << "------------------------------------\n";

            // Información de la operación
            ticket << "<span weight=\"bold\">Folio:</span> " << log->m_id << "\n";

            if (db_->get_item(5)->m_valor == "1")
                ticket << "<span weight=\"bold\">Atendió:</span> " << log->m_user << "\n";

            ticket << "------------------------------------\n";

            // Detalle de montos (alineados a la derecha)
            ticket << std::left << std::setw(18) << "Total:"
                   << std::right << std::setw(18) << "<span weight=\"bold\" size=\"large\">$" << log->m_total << "</span>\n";

            ticket << std::left << std::setw(18) << "Tipo de pago:"
                   << std::right << std::setw(18) << "Efectivo\n";

            ticket << std::left << std::setw(18) << "Ingreso:"
                   << std::right << std::setw(18) << "$" << log->m_ingreso << "\n";

            ticket << std::left << std::setw(18) << "Cambio:"
                   << std::right << std::setw(18) << "$" << log->m_cambio << "\n";

            if (faltante > 0)
                ticket << std::left << std::setw(18) << "<span foreground=\"red\">Faltante:</span>"
                       << std::right << std::setw(18) << "<span foreground=\"red\">$" << faltante << "</span>\n";

            ticket << "------------------------------------\n";

            // Estado destacado
            ticket << "<span size=\"large\" weight=\"bold\" foreground=\""
                   << (log->m_estatus == "Exito" ? "#2ecc71" : (log->m_estatus == "PENDIENTE" ? "#f39c12" : "#e74c3c"))
                   << "\">" << log->m_estatus << "</span>\n";

            ticket << "====================================\n";

            // Pie de página
            if (db_->get_item(6)->m_valor == "1")
                ticket << "<span size=\"small\" style=\"italic\">"
                       << json["contacto"].get<std::string>() << "</span>\n";

            if (db_->get_item(1)->m_valor == "1")
                ticket << "\n<span size=\"small\" style=\"italic\" weight=\"bold\">"
                       << json["agradecimiento"].get<std::string>() << "</span>\n";

            ticket << "\n\n\n"; // Espacio final para corte de ticket

            // Configuración de impresión
            auto print_op = PrintFormOperation::create();
            auto settings = Gtk::PrintSettings::create();
            auto page_setup = Gtk::PageSetup::create();

            print_op->set_markup(ticket.str());
            print_op->set_track_print_status(true);

            // Tamaño típico para ticket (80mm ancho × longitud variable)
            auto paper_size = Gtk::PaperSize("custom", "Ticket", 80, 297, Gtk::Unit::MM); // 80mm × 297mm (largo suficiente)
            page_setup->set_paper_size(paper_size);
            page_setup->set_orientation(Gtk::PageOrientation::PORTRAIT);
            page_setup->set_top_margin(5, Gtk::Unit::MM);
            page_setup->set_bottom_margin(5, Gtk::Unit::MM);
            page_setup->set_left_margin(5, Gtk::Unit::MM);
            page_setup->set_right_margin(5, Gtk::Unit::MM);

            print_op->set_default_page_setup(page_setup);
            print_op->set_print_settings(settings);

            try
            {
                print_op->run(Gtk::PrintOperation::Action::PRINT, *Global::Widget::v_main_window);

                print_op->signal_done().connect([](Gtk::PrintOperation::Result result)
                {
                    switch (result)
                    {
                        case Gtk::PrintOperation::Result::ERROR:
                            Global::Widget::reveal_toast("Error al imprimir", Gtk::MessageType::ERROR);
                            break;
                        case Gtk::PrintOperation::Result::APPLY:
                            Global::Widget::reveal_toast("Ticket impreso correctamente", Gtk::MessageType::INFO);
                            break;
                        case Gtk::PrintOperation::Result::CANCEL:
                            Global::Widget::reveal_toast("Impresión cancelada", Gtk::MessageType::WARNING);
                            break;
                        default:
                            break;
                    } 
                });
            }
            catch (const Gtk::PrintError &ex)
            {
                Global::Widget::reveal_toast("Error de impresión: " + std::string(ex.what()), Gtk::MessageType::ERROR);
                std::cerr << "Error en impresión: " << ex.what() << std::endl;
            }
        }
    }
}