#include "view/configuracion/usuarios.hpp"

VUsuarios::VUsuarios(BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &refBuilder) : Gtk::Box(cobject),


                                                                                              m_builder(refBuilder)
{
    v_treeview = m_builder->get_widget<Gtk::ColumnView>("tree_usuarios");

    v_btn_nuevo = m_builder->get_widget<Gtk::Button>("btn_new");
    v_btn_modificar = m_builder->get_widget<Gtk::Button>("btn_edit");
    v_btn_eliminar = m_builder->get_widget<Gtk::Button>("btn_del");

    for (size_t i = 0; i < 19; i++)
        v_chk_roles[i] = m_builder->get_widget<Gtk::CheckButton>("check_rol_" + std::to_string(i+1));
    v_chk_todos_roles = m_builder->get_widget<Gtk::CheckButton>("check_main");

    v_btn_guardar_roles = m_builder->get_widget<Gtk::Button>("btn_guardar_permisos");
}

VUsuarios::~VUsuarios()
{
}

void VUsuarios::on_setup_label(const Glib::RefPtr<Gtk::ListItem> &list_item)
{
    list_item->set_child(*Gtk::make_managed<Gtk::Label>("", Gtk::Align::START));
}

void VUsuarios::on_setup_row(const Glib::RefPtr<Gtk::ListItem> &list_item)
{
    auto col = std::dynamic_pointer_cast<MUsuarios>(list_item->get_item());
    auto label = dynamic_cast<Gtk::Label *>(list_item->get_child());

    label->set_text(Glib::ustring::format(col->m_usuario));
}
