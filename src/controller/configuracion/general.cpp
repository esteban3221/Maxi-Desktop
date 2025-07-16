#include "controller/configuracion/general.hpp"

General::General(BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &refBuilder) : VGeneral(cobject, refBuilder)
{
    signal_map().connect(sigc::mem_fun(*this, &General::on_show_mapping));
    v_ety_mensaje_inicio->signal_changed().connect(sigc::mem_fun(*this, &General::on_ety_change_mensaje_inicio));
    v_btn_reinicia_val->signal_clicked().connect(sigc::mem_fun(*this, &General::on_btn_reinicia_val_clicked));
    v_btn_actualiza_pos->signal_clicked().connect(sigc::mem_fun(*this, &General::on_btn_actualiza_pos_clicked));
    v_btn_retirada->signal_clicked().connect(sigc::mem_fun(*this, &General::on_btn_retirada));
}

General::~General()
{
}

void General::on_show_mapping()
{
    auto db = std::make_unique<Configuracion>();
    auto data = db->get_conf_data(2, 2);

    v_ety_mensaje_inicio->set_text(data->get_item(0)->m_valor);
}

void General::on_ety_change_mensaje_inicio()
{
    auto db = std::make_unique<Configuracion>();
    db->update_conf(MConfiguracion::create(2, "Mensaje Inicio", v_ety_mensaje_inicio->get_text()));
}
void General::on_btn_retirada()
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
        Global::Widget::m_refActionGroup->lookup_action("cerrarsesion")->activate();
        Global::Widget::v_revealer->set_reveal_child(false);
    });
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

    chkb->signal_toggled().connect([btn, chkb](){ btn->set_sensitive(chkb->get_active()); });
    
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
        v_dialog->close();
    });

    v_dialog->show();
}
void General::on_file_dialog_finish(const Glib::RefPtr<Gio::AsyncResult> &result, const Glib::RefPtr<Gtk::FileDialog> &dialog)
{
    try
    {
        auto file = dialog->open_finish(result);

        // Notice that this is a std::string, not a Glib::ustring.
        auto filename = file->get_path();
        std::cout << "File selected: " << filename << std::endl;

        auto future = cpr::PostAsync(cpr::Url{Global::System::URL + "configuracion/actualiza_pos"},
                                     Global::Utility::header,
                                     cpr::Multipart{{"file", cpr::File(filename)}});

        Global::Utility::consume_and_do(future, [this](cpr::Response response)
                                        {
            if (response.status_code == 200)
            {
                
                Global::Widget::reveal_toast("Exito");
            }
            std::cout << response.text << std::endl; });
    }
    catch (const Gtk::DialogError &err)
    {
        // Can be thrown by dialog->open_finish(result).
        std::cout << "No file selected. " << err.what() << std::endl;
    }
    catch (const Glib::Error &err)
    {
        std::cout << "Unexpected exception. " << err.what() << std::endl;
    }
}