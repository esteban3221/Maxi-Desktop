#include "view/menu.hpp"

VMenu::VMenu(BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &refBuilder) : Gtk::Box(cobject), m_builder(refBuilder)
{

    v_list_box_menu = m_builder->get_widget<Gtk::ListBox>("list_box_menu");
    v_stack_menu = m_builder->get_widget<Gtk::Stack>("stack_menu");
    v_stack_menu->set_transition_type(Gtk::StackTransitionType::SLIDE_UP_DOWN);
    v_list_box_menu->signal_row_activated().connect(sigc::mem_fun(*this, &VMenu::on_menu_item_clicked));

    Global::Widget::v_progress_bar = m_builder->get_widget<Gtk::ProgressBar>("progress");

    v_stack_menu->set_margin(10);
    v_stack_menu->add(* Gtk::manage(new Venta()), "0");
    v_stack_menu->add(* Gtk::manage(new PagoA()), "1");

    #ifdef _WIN32
    auto builder = Gtk::Builder::create_from_string(View::pagom_ui);
    #else
    auto builder = Gtk::Builder::create_from_file("../ui/menu/pago_m.ui");
    #endif
    auto pago_m = Gtk::Builder::get_widget_derived<PagoM>(builder, "box_pago_m");
    v_stack_menu->add(*pago_m, "2");

    #ifdef _WIN32
    builder = Gtk::Builder::create_from_string(View::refill_ui);
    #else
    builder = Gtk::Builder::create_from_file("../ui/menu/refill.ui");
    #endif
    auto refill = Gtk::Builder::get_widget_derived<Refill>(builder, "box_refill");
    v_stack_menu->add(*refill, "3");

    #ifdef _WIN32
    builder = Gtk::Builder::create_from_string(View::movimientos_ui);
    #else
    builder = Gtk::Builder::create_from_file("../ui/menu/movimientos.ui");
    #endif
    auto movimientos = Gtk::Builder::get_widget_derived<Movimientos>(builder, "box_movimientos");
    v_stack_menu->add(*movimientos, "4");

    v_stack_menu->add(* Gtk::manage(new VConfiguracion()), "5");

    signal_map().connect([this](){v_stack_menu->set_visible_child("0");});
}

VMenu::~VMenu()
{
}

void VMenu::on_menu_item_clicked(Gtk::ListBoxRow * row)
{
    v_stack_menu->set_visible_child(std::to_string(row->get_index()));
}

namespace View
{
    const char *menu_ui = R"(<?xml version='1.0' encoding='UTF-8'?>
<!-- Created with Cambalache 0.96.0 -->
<interface>
  <!-- interface-name menu.ui -->
  <requires lib="gtk" version="4.0"/>
  <object class="GtkBox" id="box_menu">
    <child>
      <object class="GtkScrolledWindow" id="scrolled_window">
        <property name="hscrollbar-policy">never</property>
        <property name="propagate-natural-width">True</property>
        <property name="vexpand">true</property>
        <child>
          <object class="GtkListBox" id="list_box_menu">
            <property name="activate-on-single-click">true</property>
            <property name="selection-mode">single</property>
            <property name="valign">start</property>
            <child>
              <object class="GtkBox">
                <property name="spacing">10</property>
                <child>
                  <object class="GtkImage">
                    <property name="icon-name">object-merge-symbolic</property>
                  </object>
                </child>
                <child>
                  <object class="GtkLabel">
                    <property name="label">Venta</property>
                  </object>
                </child>
              </object>
            </child>
            <child>
              <object class="GtkBox">
                <property name="spacing">10</property>
                <child>
                  <object class="GtkImage">
                    <property name="icon-name">outbox-symbolic</property>
                  </object>
                </child>
                <child>
                  <object class="GtkLabel">
                    <property name="label">Pago Automatico</property>
                  </object>
                </child>
              </object>
            </child>
            <child>
              <object class="GtkBox">
                <property name="spacing">10</property>
                <child>
                  <object class="GtkImage">
                    <property name="icon-name">outbox-symbolic</property>
                  </object>
                </child>
                <child>
                  <object class="GtkLabel">
                    <property name="label">Pago manual</property>
                  </object>
                </child>
              </object>
            </child>
            <child>
              <object class="GtkBox">
                <property name="spacing">10</property>
                <child>
                  <object class="GtkImage">
                    <property name="icon-name">xapp-prefs-display-symbolic</property>
                  </object>
                </child>
                <child>
                  <object class="GtkLabel">
                    <property name="label">Carga / Retirada</property>
                  </object>
                </child>
              </object>
            </child>
            <child>
              <object class="GtkBox">
                <property name="spacing">10</property>
                <child>
                  <object class="GtkImage">
                    <property name="icon-name">sidebar-tree-symbolic</property>
                  </object>
                </child>
                <child>
                  <object class="GtkLabel">
                    <property name="label">Movimientos</property>
                  </object>
                </child>
              </object>
            </child>
            <child>
              <object class="GtkBox">
                <property name="spacing">10</property>
                <child>
                  <object class="GtkImage">
                    <property name="icon-name">configure-symbolic</property>
                  </object>
                </child>
                <child>
                  <object class="GtkLabel">
                    <property name="label">Configuracion</property>
                  </object>
                </child>
              </object>
            </child>
            <style>
              <class name="navigation-sidebar"/>
            </style>
          </object>
        </child>
      </object>
    </child>
    <child>
      <object class="GtkSeparator"/>
    </child>
    <child>
      <object class="GtkBox">
        <property name="orientation">vertical</property>
        <child>
          <object class="GtkProgressBar" id="progress"/>
        </child>
        <child>
          <object class="GtkStack" id="stack_menu">
            <property name="hexpand">true</property>
            <property name="vexpand">True</property>
          </object>
        </child>
      </object>
    </child>
  </object>
</interface>
)";
} // namespace View
