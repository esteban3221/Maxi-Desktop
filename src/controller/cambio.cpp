#include "controller/cambio.hpp"

Cambio::Cambio(BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &refBuilder) : VCambio(cobject, refBuilder)
{
    v_switch_select->property_active().signal_changed().connect(sigc::mem_fun(*this, &Cambio::on_switch_changed));
}

Cambio::~Cambio()
{
}

void Cambio::on_switch_changed()
{
    if (not v_switch_select->get_active())
        v_stack_selector->set_visible_child("pago_manual");
    else
        v_stack_selector->set_visible_child("pago");
}