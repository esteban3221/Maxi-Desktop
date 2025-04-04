#include "view/configuracion/impresora.hpp"

VImpresora::VImpresora(BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &refBuilder) : Gtk::Box(cobject),m_builder(refBuilder)
{
}

VImpresora::~VImpresora()
{
}