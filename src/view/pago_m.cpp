#include "view/pago_m.hpp"

VPagoM::VPagoM(BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &refBuilder) : Gtk::Box(cobject),
                                                                                       m_builder(refBuilder)
{
}

VPagoM::~VPagoM()
{
}
