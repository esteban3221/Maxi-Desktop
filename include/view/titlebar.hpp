#pragma once
#include <gtkmm.h>
#include "global.hpp"
#include "model/list_ip.hpp"

class VTitlebar : public Gtk::HeaderBar
{
private:
    Glib::RefPtr<Gtk::Builder> m_builder;

protected:
    Gtk::MenuButton *v_menu_status, *v_menu_titlebar;
    Gtk::Button *v_btn_regresar;
    Gtk::Popover *v_popover_titlebar;
    Gtk::Entry *v_ety_servidor;
    Gtk::ListBox *v_list_ip;
    Gtk::Menu *v_window_menu;

    class ListItem : public Gtk::ListBoxRow
    {
    private:
        /* data */
    public:
        ListItem(const Glib::RefPtr<MListIp> &);
        ~ListItem();
    };

public:
    VTitlebar(BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &refBuilder);
    ~VTitlebar();
};

namespace View
{
    extern const char *titlebar_ui;
}
