#include "view/titlebar.hpp"
#include "titlebar.hpp"

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
    Global::Widget::v_main_title->set_text("Maxicajero");
}

VTitlebar::~VTitlebar()
{
}

VTitlebar::ListItem::ListItem(const Glib::RefPtr<MListIp> &list_ip)
{
    auto box = Gtk::manage(new Gtk::Box());
    auto lbl_ip = Gtk::manage(new Gtk::Label(list_ip->m_ip));
    auto btn_del = Gtk::manage(new Gtk::Button());
    

    box->append(*lbl_ip);
    box->append(*btn_del);

    btn_del->set_image_from_icon_name("list-remove-symbolic");
    btn_del->set_css_classes({"destructive-action"});

    lbl_ip->set_halign(Gtk::Align::START);
    lbl_ip->set_hexpand(true);

    btn_del->signal_clicked().connect([this, list_ip]() {
        auto bd = std::make_unique<ListIp>();
        bd->delete_ip(list_ip);
        ((Gtk::ListBox *)get_parent())->remove(*this);
    });

    set_child(*box);
}

VTitlebar::ListItem::~ListItem()
{
    
}