#pragma once
#include <gtkmm.h>
#include "global.hpp"

class VMovimientos : public Gtk::Box
{
private:
    Glib::RefPtr<Gtk::Builder> m_builder;
protected:
public:
    VMovimientos(BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &refBuilder);
    ~VMovimientos();
};