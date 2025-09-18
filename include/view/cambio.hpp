#pragma once
#include <gtkmm.h>
#include "controller/cambio_a.hpp"
#include "controller/cambio_m.hpp"

class VCambio : public Gtk::Box
{
private:
    Glib::RefPtr<Gtk::Builder> m_builder;
protected:
    Gtk::Switch *v_switch_select = nullptr;
    Gtk::Stack *v_stack_selector = nullptr;

public:
    VCambio(BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &refBuilder);
    ~VCambio();
};

namespace View
{
   extern const char *ui_cambio;
}
