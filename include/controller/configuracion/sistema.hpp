#pragma once
#include "view/configuracion/sistema.hpp"

class Sistema : public VSistema
{
private:
    /* data */
public:
    Sistema(BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &refBuilder);
    ~Sistema();
};
