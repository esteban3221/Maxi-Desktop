#pragma once
#include <gtkmm.h>
#include "global.hpp"
#include "model/usuarios.hpp"
#include "model/usuarios_roles.hpp"


class VUsuarios : public Gtk::Box
{
private:
    Glib::RefPtr<Gtk::Builder> m_builder;

protected:
    Gtk::ColumnView *v_treeview;
    Gtk::Button *v_btn_nuevo;
    Gtk::Button *v_btn_modificar;
    Gtk::Button *v_btn_eliminar;

    Gtk::CheckButton *v_chk_roles[19]{nullptr};
    Gtk::CheckButton *v_chk_todos_roles;
    Gtk::Button *v_btn_guardar_roles;

    void on_setup_label(const Glib::RefPtr<Gtk::ListItem> &list_item);
    void on_setup_row(const Glib::RefPtr<Gtk::ListItem> &list_item);

    std::unique_ptr<Gtk::MessageDialog> v_dialog;
public:
    VUsuarios(BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &refBuilder);
    ~VUsuarios();
};


namespace View
{
    namespace Conf
    {
        extern const char *usuario_ui;
    } // namespace Conf
} // namespace View