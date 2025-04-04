#pragma once
#include <gtkmm.h>
#include "global.hpp"

class VUsuarios : public Gtk::Box
{
private:
    Glib::RefPtr<Gtk::Builder> m_builder;

public:
    VUsuarios(BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &refBuilder);
    ~VUsuarios();
};
