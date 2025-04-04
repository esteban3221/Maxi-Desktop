#pragma once
#include "view/configuracion/sistema.hpp"

class Sistema : public VSistema
{
private:
    void on_show_map();
public:
    Sistema(BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &refBuilder);
    ~Sistema();
};
