#include "view/bloq_num.hpp"

VBaseNip::VBaseNip(BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &refBuilder) : m_builder{refBuilder},
                                                                                            Gtk::Box(cobject)
{
    this->v_btn_nip_back = m_builder->get_widget<Gtk::Button>("btn_nip_back");
    this->v_ety_spin = m_builder->get_widget<Gtk::SpinButton>("ety_spin");
    this->v_btn_nip_del = m_builder->get_widget<Gtk::Button>("btn_nip_del");
    this->v_btn_nip_enter = m_builder->get_widget<Gtk::Button>("btn_nip_enter");

    for (size_t i = 0; i < 10; i++)
    {
        this->v_botonera_nip[i] = m_builder->get_widget<Gtk::Button>("btn_nip_" + std::to_string(i));
        this->v_botonera_nip[i]->signal_clicked().connect([this, i](){ v_ety_spin->set_text(v_ety_spin->get_text() + std::to_string(i)); });
    }

    v_btn_nip_del->signal_clicked().connect([this](){v_ety_spin->set_text("");});
    v_ety_spin->set_numeric(true);
}

VBaseNip::~VBaseNip()
{
}


namespace View
{
    const char *bloq_num_ui = R"(<?xml version='1.0' encoding='UTF-8'?>
<!-- Created with Cambalache 0.95.0 -->
<interface>
  <!-- interface-name bloq_numerico.ui -->
  <requires lib="gtk" version="4.0"/>
  <object class="GtkBox" id="box_nip">
    <property name="homogeneous">false</property>
    <property name="margin-bottom">10</property>
    <property name="margin-end">100</property>
    <property name="margin-start">100</property>
    <property name="margin-top">10</property>
    <property name="orientation">vertical</property>
    <property name="spacing">30</property>
    <child>
      <object class="GtkSpinButton" id="ety_spin">
        <property name="adjustment">
          <object class="GtkAdjustment">
            <property name="lower">1.0</property>
            <property name="page-increment">1.0</property>
            <property name="page-size">1.0</property>
            <property name="step-increment">1.0</property>
            <property name="upper">100000.0</property>
          </object>
        </property>
        <property name="enable-undo">False</property>
        <property name="halign">fill</property>
        <property name="height-request">100</property>
        <property name="hexpand">true</property>
        <property name="max-width-chars">8</property>
        <property name="numeric">True</property>
        <property name="snap-to-ticks">True</property>
        <property name="text">1</property>
        <property name="update-policy">if-valid</property>
        <property name="value">1.0</property>
        <property name="wrap">True</property>
        <property name="xalign">0.5</property>
        <style>
          <class name="title-1"/>
        </style>
      </object>
    </child>
    <child>
      <object class="GtkGrid">
        <property name="column-homogeneous">true</property>
        <property name="column-spacing">10</property>
        <property name="halign">fill</property>
        <property name="row-homogeneous">true</property>
        <property name="row-spacing">10</property>
        <property name="vexpand">true</property>
        <child>
          <object class="GtkButton" id="btn_nip_1">
            <property name="label">1</property>
            <layout>
              <property name="column">0</property>
              <property name="row">0</property>
            </layout>
          </object>
        </child>
        <child>
          <object class="GtkButton" id="btn_nip_2">
            <property name="label">2</property>
            <layout>
              <property name="column">1</property>
              <property name="row">0</property>
            </layout>
          </object>
        </child>
        <child>
          <object class="GtkButton" id="btn_nip_3">
            <property name="label">3</property>
            <layout>
              <property name="column">2</property>
              <property name="row">0</property>
            </layout>
          </object>
        </child>
        <child>
          <object class="GtkButton" id="btn_nip_4">
            <property name="label">4</property>
            <layout>
              <property name="column">0</property>
              <property name="row">1</property>
            </layout>
          </object>
        </child>
        <child>
          <object class="GtkButton" id="btn_nip_5">
            <property name="label">5</property>
            <layout>
              <property name="column">1</property>
              <property name="row">1</property>
            </layout>
          </object>
        </child>
        <child>
          <object class="GtkButton" id="btn_nip_6">
            <property name="label">6</property>
            <layout>
              <property name="column">2</property>
              <property name="row">1</property>
            </layout>
          </object>
        </child>
        <child>
          <object class="GtkButton" id="btn_nip_7">
            <property name="label">7</property>
            <layout>
              <property name="column">0</property>
              <property name="row">2</property>
            </layout>
          </object>
        </child>
        <child>
          <object class="GtkButton" id="btn_nip_8">
            <property name="label">8</property>
            <layout>
              <property name="column">1</property>
              <property name="row">2</property>
            </layout>
          </object>
        </child>
        <child>
          <object class="GtkButton" id="btn_nip_9">
            <property name="label">9</property>
            <layout>
              <property name="column">2</property>
              <property name="row">2</property>
            </layout>
          </object>
        </child>
        <child>
          <object class="GtkButton" id="btn_nip_0">
            <property name="label">0</property>
            <layout>
              <property name="column">1</property>
              <property name="row">3</property>
            </layout>
          </object>
        </child>
        <child>
          <object class="GtkButton" id="btn_nip_del">
            <property name="icon-name">edit-delete-symbolic</property>
            <layout>
              <property name="column">3</property>
              <property name="row">3</property>
            </layout>
          </object>
        </child>
        <child>
          <object class="GtkButton" id="btn_nip_enter">
            <property name="icon-name">go-jump-symbolic</property>
            <layout>
              <property name="column">3</property>
              <property name="row">0</property>
              <property name="row-span">3</property>
            </layout>
          </object>
        </child>
        <child>
          <object class="GtkButton" id="btn_nip_back">
            <property name="icon-name">go-previous-symbolic</property>
            <layout>
              <property name="column">0</property>
              <property name="row">3</property>
            </layout>
          </object>
        </child>
        <style>
          <class name="title-1"/>
        </style>
      </object>
    </child>
  </object>
</interface>
)";
} // namespace View