#include "view/menu.hpp"

VMenu::VMenu(BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &refBuilder) : Gtk::Box(cobject), m_builder(refBuilder)
{

	v_list_box_menu = m_builder->get_widget<Gtk::ListBox>("list_box_menu");
	v_stack_menu = m_builder->get_widget<Gtk::Stack>("stack_menu");
	v_stack_menu->set_transition_type(Gtk::StackTransitionType::SLIDE_UP_DOWN);
	v_list_box_menu->signal_row_activated().connect(sigc::mem_fun(*this, &VMenu::on_menu_item_clicked));

  auto builder_default = Gtk::Builder::create_from_string(View::default_ui);
  auto default_view = builder_default->get_widget<Gtk::Box>("box_default");
  v_lbl_user = builder_default->get_widget<Gtk::Label>("lbl_user");

	Global::Widget::v_progress_bar = m_builder->get_widget<Gtk::ProgressBar>("progress");

	v_stack_menu->set_margin(10);

  v_stack_menu->add(*default_view, "0");

	v_stack_menu->add(*Gtk::manage(new Venta(false)), "1");
  v_stack_menu->add(*Gtk::manage(new Venta(true)), "2");
	v_stack_menu->add(*Gtk::manage(new PagoA()), "3");

	auto builder = Gtk::Builder::create_from_string(View::pagom_ui);
	auto pago_m = Gtk::Builder::get_widget_derived<PagoM>(builder, "box_pago_m");
	v_stack_menu->add(*pago_m, "4");

  builder = Gtk::Builder::create_from_string(View::ui_cambio);
  auto cambio = Gtk::Builder::get_widget_derived<Cambio>(builder, "box");
  v_stack_menu->add(*cambio, "5");

	builder = Gtk::Builder::create_from_string(View::refill_ui);
	auto refill = Gtk::Builder::get_widget_derived<Refill>(builder, "box_refill");
	v_stack_menu->add(*refill, "6");

	builder = Gtk::Builder::create_from_string(View::movimientos_ui);
	auto movimientos = Gtk::Builder::get_widget_derived<Movimientos>(builder, "box_movimientos");
	v_stack_menu->add(*movimientos, "7");

	v_stack_menu->add(*Gtk::manage(new VConfiguracion()), "8");

	signal_map().connect(sigc::mem_fun(*this, &VMenu::on_show_map));
}

VMenu::~VMenu()
{
}

void VMenu::on_menu_item_clicked(Gtk::ListBoxRow *row)
{
	v_stack_menu->set_visible_child(std::to_string(row->get_index()+1));
  v_list_box_menu->set_sensitive(false);
}

void VMenu::on_show_map()
{
  v_lbl_user->set_text("Bienvenido " + Global::User::Current);
    v_list_box_menu->unselect_all();

    std::string vista_destino = "0";

    auto result = Database::getInstance().sqlite3->command(
        "SELECT activo, vista FROM inicio_default WHERE id_usuario = ?", Global::User::id
    );

    if (result->contains("activo") && result->at("activo")[0] == "1")
    {
      std::string vista_db = result->at("vista")[0];

      if (vista_db == "Venta")
        vista_destino = "1";
      else if (vista_db == "Ingreso")
        vista_destino = "2";
      else if (vista_db == "Pago Manual")
        vista_destino = "4";
      else if (vista_db == "Refill/Consulta Niveles")
        vista_destino = "6";
    }

    v_stack_menu->set_visible_child(vista_destino);

    if (vista_destino == "0")
    {
      v_list_box_menu->set_sensitive(true);
    }
    else
    {
      // Selecciona visualmente la fila en el menú lateral y aplica la misma sensibilidad que on_menu_item_clicked
      int row_index = std::stoi(vista_destino) - 1;
      if (auto row = v_list_box_menu->get_row_at_index(row_index))
        v_list_box_menu->select_row(*row);

      v_list_box_menu->set_sensitive(false);
    }
}

namespace View
{
  const char *default_ui = R"(
  <?xml version="1.0" encoding="UTF-8"?>
<interface>
  <requires lib="gtk" version="4.0"/>
  <object class="GtkBox" id="box_default">
    <property name="orientation">1</property>
    <property name="halign">3</property>
    <property name="valign">3</property>
    <property name="spacing">15</property>
    <child>
      <object class="GtkLabel" id="lbl_user">
        <property name="label" translatable="yes">Bienvenido</property>
        <property name="css-classes">title-1</property>
      </object>
    </child>
    <child>
      <object class="GtkImage">
        <property name="icon-name">sidebar-show-symbolic</property>
        <property name="pixel-size">200</property>
      </object>
    </child>
    <child>
      <object class="GtkLabel">
        <property name="label" translatable="yes">Escoga una opcion del menu lateral</property>
        <property name="css-classes">title-4
dim-label</property>
      </object>
    </child>
  </object>
</interface>
)";

	const char *menu_ui = R"(<?xml version='1.0' encoding='UTF-8'?>
<!-- Created with Cambalache 0.96.1 -->
<interface>
  <!-- interface-name menu.ui -->
  <requires lib="gtk" version="4.0"/>
  <object class="GtkBox" id="box_menu">
    <child>
      <object class="GtkScrolledWindow" id="scrolled_window">
        <property name="hscrollbar-policy">never</property>
        <property name="propagate-natural-width">true</property>
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
                    <property name="icon-name">applications-utilities-symbolic</property>
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
                    <property name="icon-name">applications-utilities-symbolic</property>
                  </object>
                </child>
                <child>
                  <object class="GtkLabel">
                    <property name="label">Ingreso</property>
                  </object>
                </child>
              </object>
            </child>
            <child>
              <object class="GtkBox">
                <property name="spacing">10</property>
                <child>
                  <object class="GtkImage">
                    <property name="icon-name">applications-utilities-symbolic</property>
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
                    <property name="icon-name">applications-utilities-symbolic</property>
                  </object>
                </child>
                <child>
                  <object class="GtkLabel">
                    <property name="label">Pago Manual</property>
                  </object>
                </child>
              </object>
            </child>
            <child>
              <object class="GtkBox">
                <property name="spacing">10</property>
                <child>
                  <object class="GtkImage">
                    <property name="icon-name">go-jump-symbolic</property>
                  </object>
                </child>
                <child>
                  <object class="GtkLabel">
                    <property name="label">Cambio</property>
                  </object>
                </child>
              </object>
            </child>
            <child>
              <object class="GtkBox">
                <property name="spacing">10</property>
                <child>
                  <object class="GtkImage">
                    <property name="icon-name">emoji-symbols-symbolic</property>
                  </object>
                </child>
                <child>
                  <object class="GtkLabel">
                    <property name="label">Carga</property>
                  </object>
                </child>
              </object>
            </child>
            <child>
              <object class="GtkBox">
                <property name="spacing">10</property>
                <child>
                  <object class="GtkImage">
                    <property name="icon-name">emoji-recent-symbolic</property>
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
                    <property name="icon-name">preferences-other-symbolic</property>
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
            <property name="vexpand">true</property>
          </object>
        </child>
      </object>
    </child>
  </object>
</interface>
)";
} // namespace View
