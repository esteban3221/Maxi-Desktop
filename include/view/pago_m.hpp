#pragma once
#include <gtkmm.h>
#include "global.hpp"

class VPagoM : public Gtk::Box
{
private:
    Glib::RefPtr<Gtk::Builder> m_builder;
protected:
    Gtk::SpinButton *v_spin_bill[6]{nullptr};
    Gtk::SpinButton *v_spin_coin[4]{nullptr};
    Gtk::Button *v_btn_cobrar = nullptr;

public:
    VPagoM(BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &refBuilder);
    ~VPagoM();
};