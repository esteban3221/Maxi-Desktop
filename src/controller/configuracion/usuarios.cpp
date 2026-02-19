#include "controller/configuracion/usuarios.hpp"

CUsuarios::CUsuarios(BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &refBuilder) : VUsuarios(cobject, refBuilder)
{
    init_usuarios();
    signal_map().connect(sigc::mem_fun(*this, &CUsuarios::on_show_map));
    v_treeview->signal_activate().connect(sigc::mem_fun(*this, &CUsuarios::on_row_activated));
    v_btn_nuevo->signal_clicked().connect(sigc::mem_fun(*this, &CUsuarios::on_btn_nuevo_clicked));
    v_btn_modificar->signal_clicked().connect(sigc::mem_fun(*this, &CUsuarios::on_btn_edit_clicked));
    v_btn_eliminar->signal_clicked().connect(sigc::mem_fun(*this, &CUsuarios::on_btn_delete_clicked));

    v_chk_todos_roles->signal_realize().connect(sigc::mem_fun(*this, &CUsuarios::on_checkbox_main_toggled));
    for (auto &&i : v_chk_roles) i->signal_toggled().connect(sigc::mem_fun(*this, &CUsuarios::state_group_checkbox));
    
    v_btn_guardar_roles->signal_clicked().connect(sigc::mem_fun(*this, &CUsuarios::on_btn_guardar_roles_clicked));
}

CUsuarios::~CUsuarios()
{
}

void CUsuarios::init_usuarios()
{
    auto m_list = Gio::ListStore<MUsuarios>::create();
    auto selection_model = Gtk::SingleSelection::create(m_list);
    v_treeview->set_model(selection_model);

    auto factory = Gtk::SignalListItemFactory::create();
    factory->signal_setup().connect(sigc::mem_fun(*this, &CUsuarios::on_setup_label));
    factory->signal_bind().connect(sigc::mem_fun(*this, &CUsuarios::on_setup_row));
    auto column = Gtk::ColumnViewColumn::create("Usuarios", factory);
    column->set_expand();
    v_treeview->append_column(column);
}

void CUsuarios::actualiza_data(const Glib::RefPtr<Gtk::SelectionModel> &selection, const Glib::RefPtr<Gio::ListStore<MUsuarios>> &log)
{
    auto single = std::dynamic_pointer_cast<Gtk::SingleSelection>(selection);
    auto list_store = std::dynamic_pointer_cast<Gio::ListStore<MUsuarios>>(single->get_model());

    list_store->remove_all();

    for (size_t i = 0; i < log->get_n_items(); i++)
        list_store->append(log->get_item(i));
    
}

void CUsuarios::on_show_map()
{
    auto future = cpr::GetAsync(cpr::Url{Global::System::URL, "sesion/get_all_users"}, Global::Utility::header);
    Global::Utility::consume_and_do(future, [this](const cpr::Response &response)
    {
        if (response.status_code == 200)
        {
            auto j = nlohmann::json::parse(response.text);
            auto usuarios = std::make_unique<Usuarios>();
            auto m_log = usuarios->get_usuarios(j["usuarios"]);
            auto model = v_treeview->get_model();
            actualiza_data(model, m_log);
        }
        else
        {
            auto model = v_treeview->get_model();
            actualiza_data(model, Gio::ListStore<MUsuarios>::create());
            
            Global::Widget::reveal_toast("No tiene permisos para acceder a esta seccion", (Gtk::MessageType)3);
        }
        
    });
}

void CUsuarios::on_row_activated(guint id)
{
    auto single_selection = std::dynamic_pointer_cast<Gtk::SingleSelection>(v_treeview->get_model());
    auto m_list = single_selection->get_typed_object<const MUsuarios>(id);

    auto future = cpr::PostAsync(cpr::Url{Global::System::URL, "sesion/get_all_roles_by_id"},Global::Utility::header, cpr::Body
    {
        "{ \"id_usuario\" : " + std::to_string(m_list->m_id) + "}"
    });

    Global::Utility::consume_and_do(future, [this](const cpr::Response &response)
    {
        if (response.status_code == 200)
        {
            auto j = nlohmann::json::parse(response.text);

            for (auto &&i : v_chk_roles)
                i->set_active(false);
            
            auto usuarios_roles = std::make_unique<UsuariosRoles>();
            auto m_list = usuarios_roles->get_usuario_roles_by_id(j["roles"]);

            for (size_t i = 0; i < m_list->get_n_items(); i++)
            {
                auto list = m_list->get_item(i);
                v_chk_roles[list->m_id_rol - 1]->set_active();
            }
            
        }
        else
        {
            
            Global::Widget::reveal_toast("No tiene permisos para acceder a esta seccion", (Gtk::MessageType)3);
        }
        
    });
}

void CUsuarios::lanza_dialog(const std::string &title)
{
    v_dialog.reset(new Gtk::MessageDialog(*Global::Widget::v_main_window, title, false, Gtk::MessageType::INFO,Gtk::ButtonsType::NONE, true));
    auto titlebar = Gtk::manage(new Gtk::HeaderBar());
    btn_add = Gtk::manage(new Gtk::Button());
    auto btn_cancel = Gtk::manage(new Gtk::Button());
    auto lbl_title = Gtk::manage(new Gtk::Label());
    auto v_box = Gtk::manage(new Gtk::Box(Gtk::Orientation::VERTICAL, 15));
    v_box->set_homogeneous();
    v_box->set_margin(10);
    v_entry_usuario = Gtk::manage(new Gtk::Entry());
    v_entry_contrasena = Gtk::manage(new Gtk::PasswordEntry());

    v_dialog->set_default_size(300, 200);

    v_entry_usuario->set_placeholder_text("Escriba el nombre de usuario");
    v_entry_usuario->property_primary_icon_name() = "system-users-symbolic";
    v_entry_contrasena->property_placeholder_text() = "Escriba la contraseña";
    v_entry_contrasena->set_show_peek_icon(true);

    v_box->append(*v_entry_usuario);
    v_box->append(*v_entry_contrasena);

    btn_add->set_label("Agregar");
    btn_add->set_css_classes({"suggested-action"});
    btn_cancel->set_label("Cancelar");
    btn_cancel->set_css_classes({"destructive-action"});
    lbl_title->set_label(title);

    titlebar->set_title_widget(*lbl_title);
    titlebar->pack_start(*btn_cancel);
    titlebar->pack_end(*btn_add);
    titlebar->set_show_title_buttons(false);

    v_dialog->set_child(*v_box);

    v_dialog->set_resizable();

    v_dialog->set_titlebar(*titlebar);
    v_dialog->set_transient_for(*Global::Widget::v_main_window);

    btn_cancel->signal_clicked().connect([this](){v_dialog->close();});

    v_dialog->show();
}

void CUsuarios::on_btn_nuevo_clicked()
{
    lanza_dialog("Nuevo Usuario");
    v_entry_usuario->set_editable(true);
    btn_add->signal_clicked().connect(sigc::mem_fun(*this, &CUsuarios::on_dialog_btn_add_clicked));
}

void CUsuarios::on_btn_edit_clicked()
{
    auto row = v_treeview->get_model()->get_selection()->get_minimum();
    if (row == -1)
    {
        Global::Widget::reveal_toast("Seleccione un usuario para editar", Gtk::MessageType::QUESTION);
        return;
    }

    auto single_selection = std::dynamic_pointer_cast<Gtk::SingleSelection>(v_treeview->get_model());
    auto m_list = single_selection->get_typed_object<const MUsuarios>(row);

    lanza_dialog("Edita Usuario");
    btn_add->set_label("Editar");
    btn_add->signal_clicked().connect(sigc::mem_fun(*this, &CUsuarios::on_dialog_btn_edit_clicked));
    v_entry_contrasena->property_placeholder_text() = "Escriba la nueva contraseña";
    v_entry_usuario->set_text(m_list->m_usuario);
    v_entry_usuario->set_editable(false);
}

void CUsuarios::on_btn_delete_clicked()
{
    v_dialog.reset(new Gtk::MessageDialog(*Global::Widget::v_main_window, "Eliminar Usuario", false, Gtk::MessageType::WARNING,Gtk::ButtonsType::CANCEL, true));
    auto button = v_dialog->add_button("Eliminar", Gtk::ResponseType::APPLY);
    button->set_css_classes({"destructive-action"});
    v_dialog->set_secondary_text("¿Esta seguro de eliminar el usuario?");

    v_dialog->signal_response().connect(sigc::mem_fun(*this, &CUsuarios::on_dialog_btn_delete_clicked));
    v_dialog->set_transient_for(*Global::Widget::v_main_window);
    v_dialog->show();
}

void CUsuarios::on_dialog_btn_edit_clicked()
{
    if (v_entry_usuario->get_text().empty() || v_entry_contrasena->get_text().empty())
    {
        
        Global::Widget::reveal_toast("Los campos no pueden estar vacios", Gtk::MessageType::WARNING);
        return;
    }

    auto row = v_treeview->get_model()->get_selection()->get_minimum();

    auto single_selection = std::dynamic_pointer_cast<Gtk::SingleSelection>(v_treeview->get_model());
    auto m_list = single_selection->get_typed_object<const MUsuarios>(row);

    auto json = nlohmann::json
    {
        {"id", m_list->m_id},
        {"username", v_entry_usuario->get_text()},
        {"password", v_entry_contrasena->get_text()}
    };

    auto future = cpr::PostAsync(cpr::Url{Global::System::URL, "sesion/modifica_usuario"},Global::Utility::header, cpr::Body{json.dump()});

    Global::Utility::consume_and_do(future, [this](const cpr::Response &response)
    {
        if (response.status_code == 200)
        {
            
            Global::Widget::reveal_toast("Usuario editado correctamente");
            v_dialog->close();
        }
        else
            Global::Widget::reveal_toast("No tiene permisos para acceder a esta seccion", (Gtk::MessageType)3);
    });


}

void CUsuarios::on_dialog_btn_delete_clicked(int response)
{
    if (response == Gtk::ResponseType::APPLY)
    {
        auto row = v_treeview->get_model()->get_selection()->get_minimum();
        if (row == -1)
        {
            Global::Widget::reveal_toast("Seleccione un usuario para eliminar", Gtk::MessageType::WARNING);
            return;
        }

        auto single_selection = std::dynamic_pointer_cast<Gtk::SingleSelection>(v_treeview->get_model());
        auto m_list = single_selection->get_typed_object<const MUsuarios>(row);

        auto json = nlohmann::json
        {
            {"id", m_list->m_id}
        };

        auto future = cpr::PostAsync(cpr::Url{Global::System::URL, "sesion/baja_usuario"},Global::Utility::header, cpr::Body{json.dump()});

        Global::Utility::consume_and_do(future, [this](const cpr::Response &response)
        {
            if (response.status_code == 200)
            {
                
                Global::Widget::reveal_toast("Usuario eliminado correctamente");
                on_show_map();
                v_dialog->close();
            }
            else
                Global::Widget::reveal_toast(response.text);
        });
    }
    else
        v_dialog->close();
    
}

void CUsuarios::on_checkbox_main_toggled()
{
    if (v_chk_todos_roles->get_active()) for (auto &&i : v_chk_roles)
            i->set_active();
    else for (auto &&i : v_chk_roles)
            i->set_active(false);
}

void CUsuarios::state_group_checkbox()
{
    int count = 0;
    for (auto &&i : v_chk_roles)if (i->get_active())
            count++;
    if (count == 19)
    {
        v_chk_todos_roles->set_active();
    }
    else if (count == 0)
    {
        v_chk_todos_roles->set_active(false);
    }
}

void CUsuarios::on_btn_guardar_roles_clicked()
{
    auto row = v_treeview->get_model()->get_selection()->get_minimum();

    auto single_selection = std::dynamic_pointer_cast<Gtk::SingleSelection>(v_treeview->get_model());
    auto m_list = single_selection->get_typed_object<const MUsuarios>(row);

    auto json = nlohmann::json
    {
        {"id_usuario", m_list->m_id},
        {"roles", {}}
    };

    for (size_t i = 0; i < 19; i++)
    {
        if (v_chk_roles[i]->get_active())
            json["roles"].push_back(i + 1);
    }

    auto future = cpr::PostAsync(cpr::Url{Global::System::URL, "sesion/modifica_usuario_roles"},Global::Utility::header, cpr::Body{json.dump()});

    Global::Utility::consume_and_do(future, [this](const cpr::Response &response)
    {
        if (response.status_code == 200)
            Global::Widget::reveal_toast("Roles guardados correctamente");
        else
            Global::Widget::reveal_toast("No tiene permisos para acceder a esta seccion", (Gtk::MessageType)3);
    });
}

void CUsuarios::on_dialog_btn_add_clicked()
{
    if (v_entry_usuario->get_text().empty() || v_entry_contrasena->get_text().empty())
    {
        Global::Widget::reveal_toast("Los campos no pueden estar vacios", Gtk::MessageType::WARNING);
        return;
    }

    auto future = cpr::PostAsync(cpr::Url{Global::System::URL, "sesion/alta_usuario"},Global::Utility::header, cpr::Body
    {
        "{ \"username\" : \"" + v_entry_usuario->get_text() + "\", \"password\" : \"" + v_entry_contrasena->get_text() + "\"}"
    });

    Global::Utility::consume_and_do(future, [this](const cpr::Response &response)
    {
        if (response.status_code == 200)
            on_show_map();
        else
            Global::Widget::reveal_toast("No tiene permisos para acceder a esta seccion", (Gtk::MessageType)3);
        v_dialog->close();
    });
}