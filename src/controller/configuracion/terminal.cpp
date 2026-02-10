#include "controller/configuracion/terminal.hpp"

Terminal::Terminal(BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &refBuilder) : VTerminal(cobject, refBuilder)
{
    signal_map().connect(sigc::mem_fun(*this, &Terminal::on_show_map));
    v_btn_add_terminal->signal_clicked().connect(sigc::mem_fun(*this, &Terminal::on_button_add_clicked));

    auto detail_terminal_builder = Gtk::Builder::create_from_string(View::Conf::details_terminal_ui);
    details_terminal = detail_terminal_builder->get_widget_derived<VTerminal::VDetailsTerminal>(detail_terminal_builder, "box_details");
    details_terminal->v_btn_close_rvl->signal_clicked().connect(sigc::mem_fun(*this, &Terminal::on_button_close_rvl_clicked));

    auto form_terminal_builder = Gtk::Builder::create_from_string(View::Conf::form_terminal_ui);
    form_terminal = form_terminal_builder->get_widget_derived<VTerminal::VFormTerminal>(form_terminal_builder, "box_form");
    form_terminal->v_btn_close_rvl->signal_clicked().connect(sigc::mem_fun(*this, &Terminal::on_button_close_rvl_clicked));
    form_terminal->v_btn_verifica_token->signal_clicked().connect(sigc::mem_fun(*this, &Terminal::on_btn_verifica_token_clicked));
    form_terminal->v_list_terminales->signal_row_activated().connect(sigc::mem_fun(*this, &Terminal::on_list_terminales_row_activated));
    form_terminal->v_btn_agregar->signal_clicked().connect(sigc::mem_fun(*this, &Terminal::on_btn_agregar_clicked));
}

Terminal::~Terminal()
{
}

void Terminal::on_show_map()
{
    v_btn_add_terminal->set_visible(true);
    if (v_flow_terminals->get_first_child())
        v_flow_terminals->remove_all();
    
    auto future = cpr::GetAsync(cpr::Url{Global::System::URL + "terminales"});
    Global::Utility::consume_and_do(future, [this](cpr::Response response)
    {
        if (response.status_code == 200)
        {
            auto json_response = nlohmann::json::parse(response.text);
            for (auto &&i : json_response["terminales"])
            {
                auto card = Gtk::manage(new VCardMP(i["id"].get<std::string>(), "phonelink-ring-symbolic"));
                card->set_tipo(i["tipo"].get<std::string>() + " - " + i["modo"].get<std::string>());
                card->set_alias(i["alias"].get<std::string>());
                card->set_descripcion(i["descripcion"].get<std::string>());

                card->signal_clicked().connect(sigc::mem_fun(*this, &Terminal::on_card_mp_clicked));

                v_flow_terminals->append(*card);
            }
        }
        else
        {
            Global::Widget::reveal_toast("Hubo un error al cargar las terminales. Por favor, intenta nuevamente.");
        }
    });
}

void Terminal::on_button_add_clicked()
{
    v_rvl->set_child(*form_terminal);
    v_rvl->set_reveal_child(true);
    v_btn_add_terminal->set_visible(false);
}

void Terminal::on_card_mp_clicked()
{
    v_rvl->set_child(*details_terminal);
    v_rvl->set_reveal_child(true);
    v_btn_add_terminal->set_visible(false);
}

void Terminal::on_button_close_rvl_clicked()
{
    v_rvl->set_reveal_child(false);
    Glib::signal_timeout().connect_once([this]()
    {
        v_rvl->unset_child();
    }, v_rvl->get_transition_duration());
    v_btn_add_terminal->set_visible(true);
}

void Terminal::on_btn_verifica_token_clicked()
{
    auto future = cpr::GetAsync(cpr::Url{"https://api.mercadopago.com/terminals/v1/list"},
                                cpr::Header{{"Authorization", "Bearer " + form_terminal->v_ety_token->get_text()}});

    Global::Utility::consume_and_do(future, [this](cpr::Response response)
    {
        if (response.status_code == 200)
        {
            form_terminal->v_ety_token->set_sensitive(false);
            auto json_response = nlohmann::json::parse(response.text);
            auto total_terminales = json_response["paging"]["total"].get<int>();
            form_terminal->v_lbl_total_terminales->set_text("Total terminales: " + std::to_string(total_terminales));
            form_terminal->v_ety_token->set_icon_from_icon_name("dialog-ok-symbolic");
            form_terminal->v_btn_verifica_token->set_sensitive(false);
            form_terminal->v_ety_token->set_icon_tooltip_text("Token válido");

            for (auto &&i : json_response["data"]["terminals"])
            {
                auto row = Gtk::manage(new VTerminal::VFormTerminal::Row(i["id"].get<std::string>(), i["operating_mode"].get<std::string>()));
                form_terminal->v_list_terminales->append(*row);
            }
        }
        else// if(response.status_code == 401)
        {
            //Token invalido
            form_terminal->v_ety_token->set_icon_from_icon_name("dialog-error-symbolic");
            form_terminal->v_ety_token->set_icon_tooltip_text("Token inválido");

            Global::Widget::reveal_toast("El valor enviado como Access Token es incorrecto. Por favor, verifícalo y vuelve a intentar realizar la petición enviando el valor correcto.");
        } 
    });
}

void Terminal::on_list_terminales_row_activated(Gtk::ListBoxRow *row)
{
    auto terminal_row = dynamic_cast<VTerminal::VFormTerminal::Row*>(row);
    if (terminal_row)
    {
        id_terminal_seleccionada = terminal_row->v_titulo->get_text();
        modo_operacion_seleccionado = terminal_row->v_subtitulo->get_text();
        form_terminal->v_btn_agregar->set_sensitive(true);
    }
}

void Terminal::on_btn_agregar_clicked()
{
    auto alias = form_terminal->v_ety_alias->get_text();
    auto descripcion = form_terminal->v_ety_descripcion->get_text();
    auto tipo = std::dynamic_pointer_cast<Gtk::StringObject>(form_terminal->v_drop_tipo->get_selected_item())->get_string();

    std::unique_ptr<Gtk::MessageDialog> dialog;

    if (modo_operacion_seleccionado == "STANDALONE")
    {
        dialog.reset(new Gtk::MessageDialog(*Global::Widget::v_main_window, "Terminal",false,Gtk::MessageType::QUESTION, Gtk::ButtonsType::YES_NO));
        dialog->set_secondary_text("La terminal seleccionada opera en modo STANDALONE, lo que significa que no se encuentra asociada a ningún punto de venta. ¿Deseas cambiar el modo de operación?");
        dialog->signal_response().connect([this, alias, descripcion, &dialog](int response_id)
        {
            if (response_id == Gtk::ResponseType::YES)
                cambia_modo_operacion(id_terminal_seleccionada, "PDV");

            dialog->close();
        });
        dialog->show();
    }

    if (alias.empty() || descripcion.empty())
    {
        dialog.reset(new Gtk::MessageDialog(*Global::Widget::v_main_window, "Terminal", false, Gtk::MessageType::QUESTION, Gtk::ButtonsType::YES_NO));
        dialog->set_secondary_text("No se ha ingresado un alias o descripción para la terminal. ¿Deseas agregar la terminal de todas formas?");
        dialog->signal_response().connect([this, &dialog, alias, descripcion, tipo](int response_id)
        {
            if (response_id == Gtk::ResponseType::NO)
                return;
            else if (response_id == Gtk::ResponseType::YES)
            {
                auto json = nlohmann::json
                {
                    {"id", id_terminal_seleccionada},
                    {"alias", alias},
                    {"tipo", tipo},
                    {"modo", modo_operacion_seleccionado},
                    {"descripcion", descripcion}
                };

                auto future = cpr::PostAsync(cpr::Url{Global::System::URL, "terminales/nueva"},
                                            cpr::Body{json.dump()});
                Global::Utility::consume_and_do(future, [this](cpr::Response response)
                {        
                    if (response.status_code == 201)
                    {            
                        Global::Widget::reveal_toast("Terminal agregada exitosamente");
                        on_button_close_rvl_clicked();
                    }
                    else 
                        Global::Widget::reveal_toast("Hubo un error al agregar la terminal. Por favor, Revise los datos ingresados e intente nuevamente.");
                });
                
                on_button_close_rvl_clicked();
            }
            
            dialog->close();
        });
        dialog->show();
    }
}

void Terminal::cambia_modo_operacion(const Glib::ustring &id, const Glib::ustring &modo_operacion)
{
    auto body = cpr::Body{R"( { "terminals": [
        {
        "id": ")" + id + R"(",
        "operating_mode": ")" + modo_operacion + R"("
        }
    ]})"};
    auto future = cpr::PostAsync(cpr::Url{"https://api.mercadopago.com/terminals/v1/setup"},
                                 cpr::Header{{"Authorization", "Bearer " + form_terminal->v_ety_token->get_text()}},
                                 body);

    Global::Utility::consume_and_do(future, [this](cpr::Response response)
    {
        if (response.status_code == 200)
        {
            Global::Widget::reveal_toast("Modo de operación cambiado exitosamente");
            form_terminal->v_list_terminales->set_sensitive(true);
        }
        else
        {
            Global::Widget::reveal_toast("Hubo un error al cambiar el modo de operación. Por favor, intenta nuevamente.");
        }
    });
}