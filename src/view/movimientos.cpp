#include "view/movimientos.hpp"
#include "movimientos.hpp"

VMovimientos::VMovimientos(BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &refBuilder) : Gtk::Box(cobject), m_builder(refBuilder)
{
    v_btn_printer = m_builder->get_widget<Gtk::Button>("btn_printer");
    v_btn_aplica_filtro = m_builder->get_widget<Gtk::Button>("btn_aplica_filtro");
    v_btn_remueve_filtros = m_builder->get_widget<Gtk::Button>("btn_remueve_filtros");
    v_dp_tipo = m_builder->get_widget<Gtk::DropDown>("dp_tipo");
    v_ety_fin = m_builder->get_widget<Gtk::Entry>("ety_fin");
    v_ety_ini = m_builder->get_widget<Gtk::Entry>("ety_ini");
    v_ety_pag = m_builder->get_widget<Gtk::Entry>("ety_pag");
    v_spin_pag = m_builder->get_widget<Gtk::SpinButton>("spin_pag");

    v_column_log = m_builder->get_widget<Gtk::ColumnView>("column_log");
    v_lbl_total_registros = m_builder->get_widget<Gtk::Label>("lbl_total_registros");

    v_pop_calendario.set_child(v_calendario);
    v_calendario.set_show_day_names();
}

VMovimientos::~VMovimientos()
{
}

void VMovimientos::on_setup_label(const Glib::RefPtr<Gtk::ListItem> &list_item)
{
    list_item->set_child(*Gtk::make_managed<Gtk::Label>("", Gtk::Align::END));
}

void VMovimientos::on_setup_label_text(const Glib::RefPtr<Gtk::ListItem> &list_item)
{
    list_item->set_child(*Gtk::make_managed<Gtk::Label>("", Gtk::Align::START));
}

void VMovimientos::on_bind_id(const Glib::RefPtr<Gtk::ListItem> &list_item)
{
    auto col = std::dynamic_pointer_cast<MLog>(list_item->get_item());
    auto label = dynamic_cast<Gtk::Label *>(list_item->get_child());

    label->set_text(Glib::ustring::format(col->m_id));
}

void VMovimientos::on_bind_usuario(const Glib::RefPtr<Gtk::ListItem> &list_item)
{
    auto col = std::dynamic_pointer_cast<MLog>(list_item->get_item());
    auto label = dynamic_cast<Gtk::Label *>(list_item->get_child());

    label->set_text(Glib::ustring::format(col->m_user));
}

void VMovimientos::on_bind_tipo(const Glib::RefPtr<Gtk::ListItem> &list_item)
{
    auto col = std::dynamic_pointer_cast<MLog>(list_item->get_item());
    auto label = dynamic_cast<Gtk::Label *>(list_item->get_child());

    label->set_text(Glib::ustring::format(col->m_tipo));
}

void VMovimientos::on_bind_ingreso(const Glib::RefPtr<Gtk::ListItem> &list_item)
{
    auto col = std::dynamic_pointer_cast<MLog>(list_item->get_item());
    auto label = dynamic_cast<Gtk::Label *>(list_item->get_child());

    label->set_text(Glib::ustring::format(col->m_ingreso));
}

void VMovimientos::on_bind_cambio(const Glib::RefPtr<Gtk::ListItem> &list_item)
{
    auto col = std::dynamic_pointer_cast<MLog>(list_item->get_item());
    auto label = dynamic_cast<Gtk::Label *>(list_item->get_child());

    label->set_text(Glib::ustring::format(col->m_cambio));
}

void VMovimientos::on_bind_total(const Glib::RefPtr<Gtk::ListItem> &list_item)
{
    auto col = std::dynamic_pointer_cast<MLog>(list_item->get_item());
    auto label = dynamic_cast<Gtk::Label *>(list_item->get_child());

    label->set_text(Glib::ustring::format(col->m_total));
}

void VMovimientos::on_bind_estatus(const Glib::RefPtr<Gtk::ListItem> &list_item)
{
    auto col = std::dynamic_pointer_cast<MLog>(list_item->get_item());
    auto label = dynamic_cast<Gtk::Label *>(list_item->get_child());

    label->set_text(Glib::ustring::format(col->m_estatus));
}

void VMovimientos::on_bind_fecha(const Glib::RefPtr<Gtk::ListItem> &list_item)
{
    auto col = std::dynamic_pointer_cast<MLog>(list_item->get_item());
    auto label = dynamic_cast<Gtk::Label *>(list_item->get_child());

    label->set_text(Glib::ustring::format(col->m_fecha.format("%A, %d de %B de %Y - Hora: %H:%M")));
}