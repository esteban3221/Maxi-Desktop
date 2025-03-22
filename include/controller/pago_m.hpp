#pragma once
#include "view/pago_m.hpp"

class PagoM : public VPagoM
{
private:
    /* data */
public:
    PagoM(BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &refBuilder);
    ~PagoM();
};