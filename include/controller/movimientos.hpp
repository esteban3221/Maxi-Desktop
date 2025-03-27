#pragma once
#include "view/movimientos.hpp"

class Movimientos : public VMovimientos
{
private:
    /* data */
public:
    Movimientos(BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &refBuilder);
    ~Movimientos();
};