#pragma once
#include "view/cambio.hpp"

class Cambio : public VCambio
{
private:
    /* data */
    void on_switch_changed();
public:
    Cambio(BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &refBuilder);
    ~Cambio();
};
