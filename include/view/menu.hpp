#pragma once
#include <gtkmm.h>
#include "global.hpp"

class VMenu : public Gtk::Box
{
protected:
    Gtk::ListBox *v_list_box_menu;
private:
Glib::RefPtr<Gtk::Builder> m_builder;
public:
    VMenu(BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &refBuilder);
    ~VMenu();
};