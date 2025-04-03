#pragma once
#include <gtkmm.h>
#include "global.hpp"

class VGeneral : public Gtk::Box
{
private:
    Glib::RefPtr<Gtk::Builder> m_builder;
public:
    VGeneral(BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &refBuilder);
    ~VGeneral();
};