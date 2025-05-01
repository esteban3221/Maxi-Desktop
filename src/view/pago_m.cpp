#include "view/pago_m.hpp"

VPagoM::VPagoM(BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &refBuilder) : Gtk::Box(cobject),
                                                                                        m_builder(refBuilder)
{
    for (size_t i = 0; i < 4; i++)
    {
        v_spin_coin[i] = m_builder->get_widget<Gtk::SpinButton>(Glib::ustring::compose("spin_coin_%1", i + 1));
        v_spin_coin[i]->set_adjustment(Gtk::Adjustment::create(0, 0, 100, 1, 10, 0));
    }
    for (size_t i = 0; i < 6; i++)
    {
        v_spin_bill[i] = m_builder->get_widget<Gtk::SpinButton>(Glib::ustring::compose("spin_bill_%1", i + 1));
        v_spin_bill[i]->set_adjustment(Gtk::Adjustment::create(0, 0, 100, 1, 10, 0));
    }
    v_btn_cobrar = m_builder->get_widget<Gtk::Button>("btn_cobrar");
}

VPagoM::~VPagoM()
{
}

namespace View
{
    const char *pagom_ui = R"(<?xml version='1.0' encoding='UTF-8'?>
<!-- Created with Cambalache 0.95.0 -->
<interface>
  <!-- interface-name pago_m.ui -->
  <requires lib="gtk" version="4.12"/>
  <object class="GtkBox" id="box_pago_m">
    <property name="orientation">vertical</property>
    <property name="spacing">10</property>
    <child>
      <object class="GtkBox">
        <property name="homogeneous">True</property>
        <property name="spacing">20</property>
        <child>
          <object class="GtkBox">
            <property name="orientation">vertical</property>
            <property name="spacing">15</property>
            <child>
              <object class="GtkLabel">
                <property name="label">Billetero</property>
                <property name="margin-bottom">20</property>
                <style>
                  <class name="title-2"/>
                </style>
              </object>
            </child>
            <child>
              <object class="GtkBox">
                <property name="homogeneous">True</property>
                <child>
                  <object class="GtkLabel">
                    <property name="label">$20</property>
                  </object>
                </child>
                <child>
                  <object class="GtkSpinButton" id="spin_bill_1">
                    <property name="halign">center</property>
                  </object>
                </child>
              </object>
            </child>
            <child>
              <object class="GtkBox">
                <property name="homogeneous">True</property>
                <child>
                  <object class="GtkLabel">
                    <property name="label">$50</property>
                  </object>
                </child>
                <child>
                  <object class="GtkSpinButton" id="spin_bill_2">
                    <property name="halign">center</property>
                  </object>
                </child>
              </object>
            </child>
            <child>
              <object class="GtkBox">
                <property name="homogeneous">True</property>
                <child>
                  <object class="GtkLabel">
                    <property name="label">$100</property>
                  </object>
                </child>
                <child>
                  <object class="GtkSpinButton" id="spin_bill_3">
                    <property name="halign">center</property>
                  </object>
                </child>
              </object>
            </child>
            <child>
              <object class="GtkBox">
                <property name="homogeneous">True</property>
                <child>
                  <object class="GtkLabel">
                    <property name="label">$200</property>
                  </object>
                </child>
                <child>
                  <object class="GtkSpinButton" id="spin_bill_4">
                    <property name="halign">center</property>
                  </object>
                </child>
              </object>
            </child>
            <child>
              <object class="GtkBox">
                <property name="homogeneous">True</property>
                <child>
                  <object class="GtkLabel">
                    <property name="label">$500</property>
                  </object>
                </child>
                <child>
                  <object class="GtkSpinButton" id="spin_bill_5">
                    <property name="halign">center</property>
                  </object>
                </child>
              </object>
            </child>
            <child>
              <object class="GtkBox">
                <property name="homogeneous">True</property>
                <child>
                  <object class="GtkLabel">
                    <property name="label">$1000</property>
                  </object>
                </child>
                <child>
                  <object class="GtkSpinButton" id="spin_bill_6">
                    <property name="halign">center</property>
                  </object>
                </child>
              </object>
            </child>
          </object>
        </child>
        <child>
          <object class="GtkBox">
            <property name="orientation">vertical</property>
            <property name="spacing">15</property>
            <child>
              <object class="GtkLabel">
                <property name="label">Monedas</property>
                <property name="margin-bottom">20</property>
                <style>
                  <class name="title-2"/>
                </style>
              </object>
            </child>
            <child>
              <object class="GtkBox">
                <property name="homogeneous">True</property>
                <child>
                  <object class="GtkLabel">
                    <property name="label">$1</property>
                  </object>
                </child>
                <child>
                  <object class="GtkSpinButton" id="spin_coin_1">
                    <property name="halign">center</property>
                  </object>
                </child>
              </object>
            </child>
            <child>
              <object class="GtkBox">
                <property name="homogeneous">True</property>
                <child>
                  <object class="GtkLabel">
                    <property name="label">$2</property>
                  </object>
                </child>
                <child>
                  <object class="GtkSpinButton" id="spin_coin_2">
                    <property name="halign">center</property>
                  </object>
                </child>
              </object>
            </child>
            <child>
              <object class="GtkBox">
                <property name="homogeneous">True</property>
                <child>
                  <object class="GtkLabel">
                    <property name="label">$5</property>
                  </object>
                </child>
                <child>
                  <object class="GtkSpinButton" id="spin_coin_3">
                    <property name="halign">center</property>
                  </object>
                </child>
              </object>
            </child>
            <child>
              <object class="GtkBox">
                <property name="homogeneous">True</property>
                <child>
                  <object class="GtkLabel">
                    <property name="label">$10</property>
                  </object>
                </child>
                <child>
                  <object class="GtkSpinButton" id="spin_coin_4">
                    <property name="halign">center</property>
                  </object>
                </child>
              </object>
            </child>
          </object>
        </child>
      </object>
    </child>
    <child>
      <object class="GtkButton" id="btn_cobrar">
        <property name="halign">center</property>
        <property name="hexpand">True</property>
        <property name="label">Cobrar $0</property>
        <property name="valign">center</property>
        <property name="vexpand">True</property>
        <style>
          <class name="suggested-action"/>
        </style>
      </object>
    </child>
  </object>
</interface>
)";
} // namespace View