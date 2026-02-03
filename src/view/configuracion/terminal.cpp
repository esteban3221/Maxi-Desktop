#include "view/configuracion/terminal.hpp"

VTerminal::VTerminal(BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &refBuilder) : Gtk::Box(cobject), m_builder(refBuilder)
{
    v_btn_add_terminal = m_builder->get_widget<Gtk::Button>("btn_agregar_terminal");
    v_flow_terminals = m_builder->get_widget<Gtk::FlowBox>("flowbox_terminales");
    v_rvl = m_builder->get_widget<Gtk::Revealer>("rvl");
    v_btn_close_rvl = m_builder->get_widget<Gtk::Button>("btn_close_rvl");
    v_lbl_titulo_rvl = m_builder->get_widget<Gtk::Label>("lbl_titulo_rvl");
}

VTerminal::~VTerminal()
{
}

namespace View
{
    namespace Conf
    {
        const char *terminal_ui = R"(<?xml version="1.0" encoding="UTF-8"?>
<interface>
  <requires lib="gtk" version="4.0"/>
  <object class="GtkBox" id="box_terminales">
    <property name="margin-bottom">16</property>
    <property name="margin-end">16</property>
    <property name="margin-start">16</property>
    <property name="margin-top">16</property>
    <property name="spacing">16</property>
    <child>
      <object class="GtkBox">
        <property name="orientation">1</property>
        <child>
          <object class="GtkBox">
            <property name="homogeneous">true</property>
            <property name="spacing">8</property>
            <child>
              <object class="GtkLabel" id="lbl_titulo">
                <property name="halign">1</property>
                <property name="label">Terminales de Pago</property>
                <property name="valign">1</property>
                <property name="xalign">0</property>
                <style>
                  <class name="title-1"/>
                </style>
              </object>
            </child>
            <child>
              <object class="GtkButton" id="btn_agregar_terminal">
                <property name="halign">2</property>
                <property name="icon-name">add</property>
                <property name="valign">3</property>
                <style>
                  <class name="suggested-action"/>
                </style>
              </object>
            </child>
          </object>
        </child>
        <child>
          <object class="GtkScrolledWindow" id="scrolled_terminales">
            <property name="hexpand">true</property>
            <property name="propagate-natural-height">true</property>
            <property name="propagate-natural-width">true</property>
            <property name="vexpand">true</property>
            <child>
              <object class="GtkFlowBox" id="flowbox_terminales">
                <property name="column-spacing">16</property>
                <property name="homogeneous">false</property>
                <property name="max-children-per-line">4</property>
                <property name="min-children-per-line">1</property>
                <property name="orientation">0</property>
                <property name="row-spacing">16</property>
                <property name="selection-mode">0</property>
              </object>
            </child>
          </object>
        </child>
      </object>
    </child>
    <child>
      <object class="GtkRevealer" id="rvl">
        <property name="reveal-child">false</property>
        <property name="transition-type">3</property>
        <property name="child">
          <object class="GtkBox">
            <property name="orientation">1</property>
            <child>
              <object class="GtkBox">
                <property name="orientation">0</property>
                <property name="homogeneous">true</property>
                <child>
                  <object class="GtkButton" id="btn_close_rvl">
                    <property name="halign">1</property>
                    <property name="valign">3</property>
                    <property name="icon-name">close</property>
                    <style>
                      <class name="circular"/>
                      <class name="destructive-action"/>
                    </style>
                  </object>
                </child>
                <child>
                  <object class="GtkLabel" id="lbl_titulo_rvl">
                    <property name="halign">3</property>
                    <property name="valign">3</property>
                    <property name="label" translatable="yes">Test</property>
                    <style>
                      <class name="title-2"/>
                    </style>
                  </object>
                </child>
              </object>
            </child>
          </object>
        </property>
      </object>
    </child>
  </object>
</interface>
)";
    } // namespace Conf
} // namespace View