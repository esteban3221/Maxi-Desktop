#include "view/movimientos.hpp"

VMovimientos::VMovimientos(BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &refBuilder) : Gtk::Box(cobject), m_builder(refBuilder)
{
}

VMovimientos::~VMovimientos()
{
}