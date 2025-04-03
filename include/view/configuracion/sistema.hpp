#pragma once
#include <gtkmm.h>
#include "global.hpp"

class VSistema : public Gtk::Box
{
private:
    Glib::RefPtr<Gtk::Builder> m_builder;

public:
    VSistema(BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &refBuilder);
    ~VSistema();
};