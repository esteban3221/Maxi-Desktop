#include "view/configuracion/empresa.hpp"

VEmpresa::VEmpresa(BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &refBuilder) : m_builder(refBuilder),
                                                                                            Gtk::Box(cobject)
{
}

VEmpresa::~VEmpresa()
{
}