#include "view/titlebar.hpp"

VTitlebar::VTitlebar(BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &refBuilder) : Gtk::HeaderBar(cobject),
                                                                                              m_builder(refBuilder)
{
    v_ety_servidor = m_builder->get_widget<Gtk::Entry>("ety_servidor");
    v_list_ip = m_builder->get_widget<Gtk::ListBox>("list_ip");
    v_btn_regresar = m_builder->get_widget<Gtk::Button>("btn_regresar");
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

    btn_del->signal_clicked().connect([this, list_ip]() 
    {
        auto bd = std::make_unique<ListIp>();
        bd->delete_ip(list_ip);
        ((Gtk::ListBox *)get_parent())->remove(*this);
    });

    set_child(*box);
}

VTitlebar::ListItem::~ListItem()
{
    
}

namespace View
{
    const char *titlebar_ui = R"(<?xml version='1.0' encoding='UTF-8'?>
<!-- Created with Cambalache 0.96.1 -->
<interface>
  <!-- interface-name titlebar.ui -->
  <requires lib="gio" version="2.0"/>
  <requires lib="gtk" version="4.0"/>
  <object class="GtkHeaderBar" id="header">
    <property name="title-widget">
      <object class="GtkLabel" id="lbl_titlebar"/>
    </property>
    <child type="start">
      <object class="GtkMenuButton" id="menu_status">
        <property name="label">Conectado</property>
        <property name="popover">popover_titlebar</property>
      </object>
    </child>
    <child type="end">
      <object class="GtkBox">
        <child>
          <object class="GtkButton" id="btn_regresar">
            <property name="icon-name">go-previous-symbolic</property>
            <property name="tooltip-markup">Cerrar Sesion (Crtl d)</property>
          </object>
        </child>
        <child type="end">
          <object class="GtkMenuButton" id="menu_titlebar">
            <property name="icon-name">open-menu-symbolic</property>
            <property name="menu-model">window_menu</property>
            <property name="primary">true</property>
            <property name="tooltip-text" translatable="yes">Main Menu</property>
          </object>
        </child>
        <style>
          <class name="linked"/>
        </style>
      </object>
    </child>
  </object>
  <menu id="window_menu">
    <section>
      <item>
        <attribute name="action">app.cerrarsesion</attribute>
        <attribute name="label">Cerrar Sesion</attribute>
      </item>
      <item>
        <attribute name="action">app.shortcuts</attribute>
        <attribute name="label" translatable="yes">Keyboard Shortcuts</attribute>
      </item>
      <item>
        <attribute name="action">app.about</attribute>
        <attribute name="label" translatable="yes">About App</attribute>
      </item>
      <item>
        <attribute name="action">app.quit</attribute>
        <attribute name="label" translatable="yes">Quit App</attribute>
      </item>
      <item>
        <attribute name="action">app.volcadolog</attribute>
        <attribute name="label">Volcar datos de Aplicación</attribute>
      </item>
    </section>
  </menu>
  <object class="GtkPopover" id="popover_titlebar">
    <property name="child">
      <object class="GtkBox" id="plain-popover-box">
        <property name="margin-bottom">10</property>
        <property name="margin-end">10</property>
        <property name="margin-start">10</property>
        <property name="margin-top">10</property>
        <property name="name">plain-popover-box</property>
        <property name="orientation">vertical</property>
        <property name="spacing">10</property>
        <child>
          <object class="GtkEntry" id="ety_servidor">
            <property name="placeholder-text">Introduzca la dirección de servidor...</property>
            <property name="primary-icon-name">network-transmit-receive-symbolic</property>
          </object>
        </child>
        <child>
          <object class="GtkLabel">
            <property name="halign">start</property>
            <property name="label">Direcciónes Guardadas</property>
            <property name="margin-top">10</property>
            <style>
              <class name="title-3"/>
            </style>
          </object>
        </child>
        <child>
          <object class="GtkScrolledWindow">
            <property name="height-request">250</property>
            <property name="propagate-natural-width">true</property>
            <property name="vexpand">true</property>
            <child>
              <object class="GtkListBox" id="list_ip">
                <style>
                  <class name="boxed-list"/>
                  <class name="rich-list"/>
                </style>
              </object>
            </child>
          </object>
        </child>
      </object>
    </property>
    <property name="has-arrow">true</property>
    <property name="name">plain_popover</property>
  </object>
</interface>
)";
}