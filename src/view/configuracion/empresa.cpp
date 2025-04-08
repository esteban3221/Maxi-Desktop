#include "view/configuracion/empresa.hpp"

VEmpresa::VEmpresa(BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &refBuilder) : m_builder(refBuilder),
                                                                                            Gtk::Box(cobject)
{
    v_datos_empresa[0] = m_builder->get_widget<Gtk::Entry>("ety_conf_razon");
    v_datos_empresa[1] = m_builder->get_widget<Gtk::Entry>("ety_conf_direccion");
    v_datos_empresa[2] = m_builder->get_widget<Gtk::Entry>("ety_conf_rfc");
    v_datos_empresa[3] = m_builder->get_widget<Gtk::Entry>("ety_conf_contacto");
    v_datos_empresa[4] = m_builder->get_widget<Gtk::Entry>("ety_conf_thanks");

    v_btn_guardar = m_builder->get_widget<Gtk::Button>("btnGuardar");

}

VEmpresa::~VEmpresa()
{
}