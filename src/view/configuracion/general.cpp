#include "view/configuracion/general.hpp"

VGeneral::VGeneral(BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &refBuilder) : Gtk::Box(cobject),
                                                                                            m_builder(refBuilder)
{
    v_ety_mensaje_inicio = m_builder->get_widget<Gtk::Entry>("ety_mensaje_inicio");
    v_btn_reinicia_val = m_builder->get_widget<Gtk::Button>("btn_reinicia_val");
}

VGeneral::~VGeneral()
{
}
