#include "view/login.hpp"

VLogin::VLogin(BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &refBuilder) : Gtk::Box(cobject),
                                                                                        m_builder(refBuilder)
{
    v_ety_passwd = m_builder->get_widget<Gtk::PasswordEntry>("ety_passwd");
    v_btn_acceder = m_builder->get_widget<Gtk::Button>("btn_acceder");
    v_lbl_titulo = m_builder->get_widget<Gtk::Label>("lbl_titulo");
}

VLogin::~VLogin()
{
}

namespace View
{
    const char *login_ui = R"(<?xml version='1.0' encoding='UTF-8'?>
<!-- Created with Cambalache 0.96.0 -->
<interface>
  <!-- interface-name login.ui -->
  <requires lib="gtk" version="4.0"/>
  <object class="GtkBox" id="box_login">
    <property name="halign">center</property>
    <property name="hexpand">true</property>
    <property name="orientation">vertical</property>
    <property name="valign">center</property>
    <property name="vexpand">true</property>
    <child>
      <object class="GtkLabel" id="lbl_titulo">
        <property name="label" translatable="yes">Titulo</property>
        <property name="margin-bottom">55</property>
        <style>
          <class name="title-1"/>
        </style>
      </object>
    </child>
    <child>
      <object class="GtkImage">
        <property name="icon-name">system-users-symbolic</property>
        <property name="pixel-size">200</property>
      </object>
    </child>
    <child>
      <object class="GtkBox">
        <property name="margin-top">35</property>
        <property name="spacing">10</property>
        <child>
          <object class="GtkPasswordEntry" id="ety_passwd">
            <property name="placeholder-text" translatable="yes">Escriba la contraseña</property>
            <property name="show-peek-icon">true</property>
            <property name="width-request">270</property>
          </object>
        </child>
        <child>
          <object class="GtkButton" id="btn_acceder">
            <property name="icon-name">go-next-symbolic</property>
            <property name="valign">end</property>
            <style>
              <class name="circular"/>
            </style>
          </object>
        </child>
      </object>
    </child>
  </object>
</interface>
)";
}