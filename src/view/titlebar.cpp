#include "view/titlebar.hpp"

VTitlebar::VTitlebar(BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &refBuilder) : Gtk::HeaderBar(cobject),
                                                                                              m_builder(refBuilder)
{
    v_ety_servidor = m_builder->get_widget<Gtk::Entry>("ety_servidor");
    v_list_ip = m_builder->get_widget<Gtk::ListBox>("list_ip");
    v_menu_status = m_builder->get_widget<Gtk::MenuButton>("menu_status");
    v_menu_titlebar = m_builder->get_widget<Gtk::MenuButton>("menu_titlebar");
    v_popover_titlebar = m_builder->get_widget<Gtk::Popover>("popover_titlebar");
    // v_window_menu = m_builder->get_widget<Gtk::Menu>("window_menu");

    Global::Widget::v_main_title = m_builder->get_widget<Gtk::Label>("lbl_titlebar");
}

VTitlebar::~VTitlebar()
{
}