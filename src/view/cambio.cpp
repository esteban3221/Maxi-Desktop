#include "view/cambio.hpp"

VCambio::VCambio(BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &refBuilder)
: Gtk::Box(cobject), m_builder(refBuilder)
{
    v_switch_select = m_builder->get_widget<Gtk::Switch>("switch_select");
    v_stack_selector = m_builder->get_widget<Gtk::Stack>("stack_selector");

    v_stack_selector->add(*(Gtk::manage(new CambioA)), "pago", "Pago");
    auto builder = Gtk::Builder::create_from_string(View::pagom_ui);
	  auto pago_m = Gtk::Builder::get_widget_derived<CambioM>(builder, "box_pago_m");
    v_stack_selector->add(*pago_m, "pago_manual", "Pago Manual");
}

VCambio::~VCambio()
{
}

namespace View
{
   const char *ui_cambio = R"(<?xml version='1.0' encoding='UTF-8'?>
<!-- Created with Cambalache 0.96.1 -->
<interface>
  <requires lib="gtk" version="4.18"/>
  <object class="GtkBox" id="box">
    <property name="margin-bottom">10</property>
    <property name="margin-end">10</property>
    <property name="margin-start">10</property>
    <property name="margin-top">10</property>
    <property name="orientation">vertical</property>
    <property name="spacing">10</property>
    <child>
      <object class="GtkBox">
        <property name="halign">baseline-center</property>
        <property name="spacing">10</property>
        <property name="valign">center</property>
        <child>
          <object class="GtkLabel">
            <property name="label">Manual</property>
          </object>
        </child>
        <child>
          <object class="GtkSwitch" id="switch_select">
            <property name="active">True</property>
            <property name="valign">center</property>
          </object>
        </child>
        <child>
          <object class="GtkLabel">
            <property name="label">Automatico</property>
          </object>
        </child>
      </object>
    </child>
    <child>
      <object class="GtkStack" id="stack_selector"/>
    </child>
  </object>
</interface>)";
}