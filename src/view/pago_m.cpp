#include "view/pago_m.hpp"

VPagoM::VPagoM(BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &refBuilder) : Gtk::Box(cobject),
                                                                                        m_builder(refBuilder)
{
    for (size_t i = 0; i < 4; i++)
    {
        v_spin_coin[i] = m_builder->get_widget<Gtk::SpinButton>(Glib::ustring::compose("spin_coin_%1", i + 1));
        v_spin_coin[i]->set_adjustment(Gtk::Adjustment::create(0, 0, 100, 1, 10, 0));
    }
    for (size_t i = 0; i < 6; i++)
    {
        v_spin_bill[i] = m_builder->get_widget<Gtk::SpinButton>(Glib::ustring::compose("spin_bill_%1", i + 1));
        v_spin_bill[i]->set_adjustment(Gtk::Adjustment::create(0, 0, 100, 1, 10, 0));
    }
    v_btn_cobrar = m_builder->get_widget<Gtk::Button>("btn_cobrar");
}

VPagoM::~VPagoM()
{
}
