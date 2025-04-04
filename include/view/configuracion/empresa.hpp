#pragma once
#include <gtkmm.h>
#include "global.hpp"

class VEmpresa : public Gtk::Box
{
private:
    /* data */
    Glib::RefPtr<Gtk::Builder> m_builder;
public:
    VEmpresa(BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &refBuilder);
    ~VEmpresa();
};
