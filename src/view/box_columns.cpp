#include "view/box_columns.hpp"

BoxColumns::BoxColumns(BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &refBuilder) : m_builder{refBuilder},
                                                                                             Gtk::Box(cobject)
{
    for (size_t i = 0; i < 3; i++)
    {
        this->v_lbl_columns[i] = m_builder->get_widget<Gtk::Label>("lbl_colum" + std::to_string(i+1));
        this->v_ety_columns[i] = m_builder->get_widget<Gtk::Entry>("ety_colum" + std::to_string(i+1));
    }
    this->v_btn_cancelar = m_builder->get_widget<Gtk::Button>("btn_cancelar");
}

BoxColumns::~BoxColumns()
{
}

namespace View
{
    const char *box_colums_ui = R"(<?xml version='1.0' encoding='UTF-8'?>
<!-- Created with Cambalache 0.96.3 -->
<interface>
  <!-- interface-name caja_total.ui -->
  <requires lib="gtk" version="4.0"/>
  <object class="GtkBox" id="box_comp_superior">
    <property name="halign">baseline-center</property>
    <property name="orientation">vertical</property>
    <property name="spacing">5</property>
    <child>
      <object class="GtkFrame">
        <property name="halign">baseline-center</property>
        <child>
          <object class="GtkGrid">
            <property name="column-spacing">10</property>
            <property name="margin-bottom">10</property>
            <property name="margin-end">10</property>
            <property name="margin-start">10</property>
            <property name="margin-top">10</property>
            <property name="row-spacing">10</property>
            <child>
              <object class="GtkLabel" id="lbl_colum1">
                <property name="label" translatable="yes">Total</property>
                <property name="margin-bottom">12</property>
                <style>
                  <class name="title-1"/>
                </style>
                <layout>
                  <property name="column">0</property>
                  <property name="row">0</property>
                </layout>
              </object>
            </child>
            <child>
              <object class="GtkEntry" id="ety_colum1">
                <property name="editable">false</property>
                <style>
                  <class name="title-4"/>
                </style>
                <layout>
                  <property name="column">0</property>
                  <property name="row">1</property>
                </layout>
              </object>
            </child>
            <child>
              <object class="GtkLabel" id="lbl_colum2">
                <property name="label" translatable="yes">Ingreso</property>
                <property name="margin-bottom">12</property>
                <style>
                  <class name="title-1"/>
                </style>
                <layout>
                  <property name="column">1</property>
                  <property name="row">0</property>
                </layout>
              </object>
            </child>
            <child>
              <object class="GtkEntry" id="ety_colum2">
                <property name="editable">false</property>
                <style>
                  <class name="title-4"/>
                </style>
                <layout>
                  <property name="column">1</property>
                  <property name="row">1</property>
                </layout>
              </object>
            </child>
            <child>
              <object class="GtkLabel" id="lbl_colum3">
                <property name="label" translatable="yes">Cambio</property>
                <property name="margin-bottom">12</property>
                <style>
                  <class name="title-1"/>
                </style>
                <layout>
                  <property name="column">2</property>
                  <property name="row">0</property>
                </layout>
              </object>
            </child>
            <child>
              <object class="GtkEntry" id="ety_colum3">
                <property name="editable">false</property>
                <style>
                  <class name="title-4"/>
                </style>
                <layout>
                  <property name="column">2</property>
                  <property name="row">1</property>
                </layout>
              </object>
            </child>
          </object>
        </child>
      </object>
    </child>
    <child>
      <object class="GtkButton" id="btn_cancelar">
        <property name="halign">baseline-center</property>
        <property name="label">Cancelar</property>
        <style>
          <class name="destructive-action"/>
          <class name="pill"/>
        </style>
      </object>
    </child>
  </object>
</interface>
)";
} // namespace View