#pragma once
#include "view/configuracion/impresora.hpp"

class Impresora : public VImpresora
{
private:
    /* data */
public:
    Impresora(BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &refBuilder);
    ~Impresora();
};