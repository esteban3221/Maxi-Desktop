#pragma once
#include "view/configuracion/usuarios.hpp"

class CUsuarios : public VUsuarios
{
private:
    void init_usuarios();
    void actualiza_data(const Glib::RefPtr<Gtk::SelectionModel> &selection, const Glib::RefPtr<Gio::ListStore<MUsuarios>> &log);
    void on_show_map();
    void on_row_activated(guint id);

    void lanza_dialog(const std::string &title);
    void on_btn_nuevo_clicked();
    void on_btn_edit_clicked();
    void on_btn_delete_clicked();

    void on_dialog_btn_add_clicked();
    void on_dialog_btn_edit_clicked();
    void on_dialog_btn_delete_clicked(int response);
    
    void on_checkbox_main_toggled();
    void state_group_checkbox();
    void on_btn_guardar_roles_clicked();

    Gtk::Button *btn_add;
    Gtk::Entry *v_entry_usuario;
    Gtk::PasswordEntry *v_entry_contrasena;

public:
    CUsuarios(BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &refBuilder);
    ~CUsuarios();
};