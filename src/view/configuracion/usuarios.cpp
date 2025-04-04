#include "view/configuracion/usuarios.hpp"

VUsuarios::VUsuarios(BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &refBuilder) : Gtk::Box(cobject),
                                                                                              m_builder(refBuilder)
{
}

VUsuarios::~VUsuarios()
{
}