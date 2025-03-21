#pragma once
#include <gtkmm.h>

class VBaseNip : public Gtk::Box
{
private:
    Glib::RefPtr<Gtk::Builder> m_builder;

protected:
    Gtk::Button *v_botonera_nip[10]{nullptr};
    Gtk::Button *v_btn_nip_del = nullptr;
    Gtk::Button *v_btn_nip_enter = nullptr;
    Gtk::Button *v_btn_nip_back = nullptr;
    Gtk::SpinButton *v_ety_spin = nullptr;

public:
    VBaseNip(BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &refBuilder);
    ~VBaseNip();
};
