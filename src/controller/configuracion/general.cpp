#include "controller/configuracion/general.hpp"

General::General(BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &refBuilder) : VGeneral(cobject, refBuilder)
{
    signal_map().connect(sigc::mem_fun(*this, &General::on_show_mapping));
    v_ety_mensaje_inicio->signal_changed().connect(sigc::mem_fun(*this, &General::on_ety_change_mensaje_inicio));
    v_btn_reiniciar_validadores->signal_clicked().connect(sigc::mem_fun(*this, &General::on_btn_reinicia_val_clicked));
    v_btn_actualizar_pos->signal_clicked().connect(sigc::mem_fun(*this, &General::on_btn_actualiza_pos_clicked));
    v_btn_retirada->signal_clicked().connect(sigc::mem_fun(*this, &General::on_btn_retirada));
    //v_btn_imagen->signal_clicked().connect(sigc::mem_fun(*this, &General::on_btn_imagen_pos_clicked));
    //v_btn_imagen_2->signal_clicked().connect(sigc::mem_fun(*this, &General::on_btn_imagen_2_clicked));
    //v_btn_desactiva_carrousel->signal_clicked().connect(sigc::mem_fun(*this, &General::on_btn_desactiva_carrousel_clicked));
    //v_chk_mostrar_notificaciones->signal_toggled().connect(sigc::mem_fun(*this, &General::on_chk_mostrar_notificaciones_toggled));
}

General::~General()
{
}

bool General::comprimir_carpeta(const std::string &carpeta_origen, const std::string &archivo_zip)
{
    // Crear archivo ZIP
    zipFile zf = zipOpen(archivo_zip.c_str(), APPEND_STATUS_CREATE);
    if (!zf)
    {
        std::cerr << "Error al crear archivo ZIP: " << archivo_zip << std::endl;
        return false;
    }

    try
    {
        // Recorrer recursivamente la carpeta
        for (const auto &entry : std::filesystem::recursive_directory_iterator(carpeta_origen))
        {
            if (entry.is_regular_file())
            {
                std::string ruta_completa = entry.path().string();
                std::string ruta_relativa = std::filesystem::relative(entry.path(), carpeta_origen).string();

                // Reemplazar separadores de ruta para compatibilidad
                std::replace(ruta_relativa.begin(), ruta_relativa.end(), '\\', '/');

                if (!agregar_archivo_a_zip(zf, ruta_completa, ruta_relativa))
                {
                    zipClose(zf, nullptr);
                    return false;
                }
            }
        }

        // Cerrar archivo ZIP
        if (zipClose(zf, nullptr) != ZIP_OK)
        {
            std::cerr << "Error al cerrar archivo ZIP" << std::endl;
            return false;
        }

        return true;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error al procesar carpeta: " << e.what() << std::endl;
        zipClose(zf, nullptr);
        return false;
    }
}

bool General::agregar_archivo_a_zip(zipFile zf, const std::string &ruta_archivo, const std::string &nombre_en_zip)
{
    // Leer contenido del archivo
    std::ifstream file(ruta_archivo, std::ios::binary | std::ios::ate);
    if (!file.is_open())
    {
        std::cerr << "No se pudo abrir archivo: " << ruta_archivo << std::endl;
        return false;
    }

    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    std::vector<char> buffer(size);
    if (!file.read(buffer.data(), size))
    {
        std::cerr << "Error al leer archivo: " << ruta_archivo << std::endl;
        return false;
    }

    // Crear entrada en el ZIP
    zip_fileinfo zipInfo = {};
    zipInfo.dosDate = 0;
    zipInfo.internal_fa = 0;
    zipInfo.external_fa = 0;

    // Abrir archivo en el ZIP
    if (zipOpenNewFileInZip(zf,
                            nombre_en_zip.c_str(),
                            &zipInfo,
                            nullptr, 0,
                            nullptr, 0,
                            nullptr,
                            Z_DEFLATED,
                            Z_DEFAULT_COMPRESSION) != ZIP_OK)
    {
        std::cerr << "Error al abrir archivo en ZIP: " << nombre_en_zip << std::endl;
        return false;
    }

    // Escribir datos en el ZIP
    if (zipWriteInFileInZip(zf, buffer.data(), static_cast<unsigned int>(size)) != ZIP_OK)
    {
        std::cerr << "Error al escribir archivo en ZIP: " << nombre_en_zip << std::endl;
        zipCloseFileInZip(zf);
        return false;
    }

    // Cerrar archivo en el ZIP
    if (zipCloseFileInZip(zf) != ZIP_OK)
    {
        std::cerr << "Error al cerrar archivo en ZIP: " << nombre_en_zip << std::endl;
        return false;
    }

    return true;
}

void General::on_show_mapping()
{
    auto db = std::make_unique<Configuracion>();
    auto data = db->get_conf_data(1, 2);

    v_sw_mostrar_notificaciones->set_active(data->get_item(0)->m_valor == "1");
    v_ety_mensaje_inicio->set_text(data->get_item(1)->m_valor);
}

void General::on_chk_mostrar_notificaciones_toggled()
{
    auto db = std::make_unique<Configuracion>();
    std::string valor = v_sw_mostrar_notificaciones->get_active() ? "1" : "0";
    db->update_conf(MConfiguracion::create(1, "Mostrar Notificaciones", valor));
}

void General::on_ety_change_mensaje_inicio()
{
    auto db = std::make_unique<Configuracion>();
    db->update_conf(MConfiguracion::create(2, "Mensaje Inicio", v_ety_mensaje_inicio->get_text()));
}

void General::on_btn_desactiva_carrousel_clicked()
{
    auto future = cpr::GetAsync(cpr::Url{Global::System::URL + "configuracion/desactiva_carrousel"}, Global::Utility::header);
    Global::Utility::consume_and_do(future, [this](cpr::Response response)
    {
        if (response.status_code == 200) 
        {
            Global::Widget::reveal_toast("Exito");
        }
        else
        {
            Global::Widget::reveal_toast("Error al desactivar el carrousel", (Gtk::MessageType)3 /*La macro de windows permea TODO LO QUE TENGA ERROR*/);
        }
    });
}

void General::on_btn_imagen_2_clicked()
{
    auto dialog = Gtk::FileDialog::create();
    dialog->set_title("Selecciona Carpeta de Carrousel");
    dialog->set_modal(true);
    dialog->select_folder(sigc::bind(sigc::mem_fun(*this, &General::on_folder_dialog_finish), dialog));
}

void General::on_folder_dialog_finish(const Glib::RefPtr<Gio::AsyncResult>& result, const Glib::RefPtr<Gtk::FileDialog>& dialog)
{
    try
    {
        auto folder = dialog->select_folder_finish(result);
        auto folder_path = folder->get_path();
        std::cout << "Folder selected: " << folder_path << std::endl;

        std::string zip_path = "carrousel.zip";
        if (comprimir_carpeta(folder_path, zip_path))
        {
            auto future = cpr::PostAsync(cpr::Url{Global::System::URL + "configuracion/sube_carpeta_pos"},
                                         Global::Utility::header,
                                         cpr::Multipart{{"file", cpr::File(zip_path)}});

            Global::Utility::consume_and_do(future, [this](cpr::Response response)
            {
                if (response.status_code == 200) 
                    Global::Widget::reveal_toast("Exito");
                else
                    Global::Widget::reveal_toast("Error al subir la carpeta", (Gtk::MessageType)3 /*La macro de windows permea TODO LO QUE TENGA ERROR*/);
                
                std::cout << response.text << std::endl; 
            });
        }
        else
        {
            Global::Widget::reveal_toast("Error al comprimir la carpeta", (Gtk::MessageType)3);
        }
    }
    catch (const Gtk::DialogError &err)
    {
        std::cout << "No folder selected. " << err.what() << std::endl;
    }
}

void General::on_btn_retirada()
{
    auto future = cpr::PostAsync(cpr::Url{Global::System::URL + "validador/retirada"}, Global::Utility::header);
    Global::Widget::reveal_toast("Retire el casette de billetes o cancele desde POS", Gtk::MessageType::WARNING);
    Global::Utility::consume_and_do(future, [this](cpr::Response response)
                                    {
        if (response.status_code == 200) 
        {
            auto j = nlohmann::json::parse(response.text);
            if (j.contains("ticket")) 
            {
                auto log = std::make_unique<Log>();
                auto m_log = log->get_log(j["ticket"]);
                auto ticket = m_log->get_item(0);

                Global::System::imprime_ticket(ticket);
            }
        }
        Global::Widget::m_refActionGroup->lookup_action("cerrarsesion")->activate();
        Global::Widget::v_revealer->set_reveal_child(false); });
}
void General::on_btn_reinicia_val_clicked()
{
    nlohmann::json json = {
        {"rol", 9},
        {"bill", {{"command", "ResetDevice"}, {"args", ""}}},
        {"coin", {{"command", "ResetDevice"}, {"args", ""}}}};
    auto future = cpr::PostAsync(cpr::Url{Global::System::URL + "configuracion/custom_command"}, Global::Utility::header, cpr::Body{json.dump()});
    Global::Utility::consume_and_do(future, [this](cpr::Response response)
                                    {
        if (response.status_code == 200) 
        {
            
            Global::Widget::reveal_toast("Exito");
        }

        std::cout << response.text << std::endl; });
}
void General::on_btn_actualiza_pos_clicked()
{
    v_dialog.reset(new Gtk::MessageDialog(*Global::Widget::v_main_window, "Actualización", false, Gtk::MessageType::QUESTION, Gtk::ButtonsType::CANCEL, true));
    auto btn = v_dialog->add_button("Continuar", Gtk::ResponseType::OK);
    btn->add_css_class({"suggested-action"});
    btn->set_sensitive(false);
    v_dialog->set_secondary_text("Esta actualización podria borrar toda configuración y datos guardados.\n"
                                 "¿Desea continuar?");

    auto chkb = Gtk::manage(new Gtk::CheckButton("Estoy consiente de la perdida completa de los datos del dispositivo."));
    v_dialog->get_content_area()->append(*chkb);

    chkb->signal_toggled().connect([btn, chkb]()
                                   { btn->set_sensitive(chkb->get_active()); });

    v_dialog->signal_response().connect([this](int response)
                                        {
        if(Gtk::ResponseType::OK == response)
        {
            auto dialog = Gtk::FileDialog::create();
            auto filters = Gio::ListStore<Gtk::FileFilter>::create();

            auto filter = Gtk::FileFilter::create();
            filter->set_name("Archivos de configuracion Zip");
            filter->add_mime_type("application/zip");
            filters->append(filter);

            auto filter_any = Gtk::FileFilter::create();
            filter_any->set_name("Archivos de configuracion");
            filter_any->add_pattern("*");
            filters->append(filter_any);

            dialog->set_filters(filters);
            dialog->open(sigc::bind(sigc::mem_fun(*this, &General::on_file_dialog_finish), dialog));
            
        }
        v_dialog->close(); });

    v_dialog->show();
}
void General::on_btn_imagen_pos_clicked()
{
    auto dialog = Gtk::FileDialog::create();
    dialog->set_title("Seleccionar Imagen de POS");
    dialog->set_modal(true);
    auto filters = Gio::ListStore<Gtk::FileFilter>::create();

    auto filter = Gtk::FileFilter::create();
    filter->set_name("PNG Image");
    filter->add_mime_type("image/png");
    filters->append(filter);

    dialog->set_filters(filters);
    dialog->open(sigc::bind(sigc::mem_fun(*this, &General::on_file_dialog_image_finish), dialog));
}
void General::on_file_dialog_image_finish(const Glib::RefPtr<Gio::AsyncResult> &result, const Glib::RefPtr<Gtk::FileDialog> &dialog)
{
    try
    {
        auto file = dialog->open_finish(result);
        auto filename = file->get_path();
        std::cout << "Image selected: " << filename << std::endl;

        auto future = cpr::PostAsync(cpr::Url{Global::System::URL + "configuracion/sube_imagen_pos"},
                                     Global::Utility::header,
                                     cpr::Multipart{{"file", cpr::File(filename)}});

        Global::Utility::consume_and_do(future, [this](cpr::Response response)
                                        {
            if (response.status_code == 200)
                Global::Widget::reveal_toast("Exito");
            else
                Global::Widget::reveal_toast("Error al subir la imagen", (Gtk::MessageType)3 /*La macro de windows permea TODO LO QUE TENGA ERROR*/);
            
            std::cout << response.text << std::endl; });
    }
    catch (const Gtk::DialogError &err)
    {
        std::cout << "No file selected. " << err.what() << std::endl;
    }
    catch (const Glib::Error &err)
    {
        std::cout << "Unexpected exception. " << err.what() << std::endl;
    }
}
void General::on_file_dialog_finish(const Glib::RefPtr<Gio::AsyncResult> &result, const Glib::RefPtr<Gtk::FileDialog> &dialog)
{
    try
    {
        auto file = dialog->open_finish(result);
        auto filename = file->get_path();
        std::cout << "File selected: " << filename << std::endl;

        auto future = cpr::PostAsync(cpr::Url{Global::System::URL + "configuracion/actualiza_pos"},
                                     Global::Utility::header,
                                     cpr::Multipart{{"file", cpr::File(filename)}});

        Global::Utility::consume_and_do(future, [this](cpr::Response response)
                                        {
            if (response.status_code == 200)
                Global::Widget::reveal_toast("Exito");
            
            std::cout << response.text << std::endl; });
    }
    catch (const Gtk::DialogError &err)
    {
        std::cout << "No file selected. " << err.what() << std::endl;
    }
    catch (const Glib::Error &err)
    {
        std::cout << "Unexpected exception. " << err.what() << std::endl;
    }
}