#pragma once
#include <gtkmm.h>
#include "global.hpp"

class VTitlebar : public Gtk::HeaderBar
{
private:
    Glib::RefPtr<Gtk::Builder> m_builder;
    Gtk::ListBoxRow *row_list_ip(const std::string &id, const Glib::ustring &label);

protected:
    Gtk::MenuButton *v_menu_status, *v_menu_titlebar;
    Gtk::Popover *v_popover_titlebar;
    Gtk::Entry *v_ety_servidor;
    Gtk::ListBox *v_list_ip;
    Gtk::Menu *v_window_menu;

public:
    VTitlebar(BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &refBuilder);
    ~VTitlebar();
};
