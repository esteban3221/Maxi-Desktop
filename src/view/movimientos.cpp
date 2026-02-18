#include "view/movimientos.hpp"

VMovimientos::VMovimientos(BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &refBuilder) : Gtk::Box(cobject), m_builder(refBuilder)
{
  v_btn_printer = m_builder->get_widget<Gtk::Button>("btn_printer");
  v_btn_imprime_corte = m_builder->get_widget<Gtk::Button>("btn_imprime_corte");
  v_btn_aplica_filtro = m_builder->get_widget<Gtk::Button>("btn_aplica_filtro");
  v_btn_remueve_filtros = m_builder->get_widget<Gtk::Button>("btn_remueve_filtros");
  v_dp_tipo = m_builder->get_widget<Gtk::DropDown>("dp_tipo");
  v_ety_fin = m_builder->get_widget<Gtk::Entry>("ety_fin");
  v_ety_ini = m_builder->get_widget<Gtk::Entry>("ety_ini");
  v_ety_pag = m_builder->get_widget<Gtk::Entry>("ety_pag");
  v_spin_pag = m_builder->get_widget<Gtk::SpinButton>("spin_pag");

  v_column_log = m_builder->get_widget<Gtk::ColumnView>("column_log");
  v_lbl_total_registros = m_builder->get_widget<Gtk::Label>("lbl_total_registros");

  v_pop_calendario.set_child(v_calendario);
  v_calendario.set_show_day_names();

  auto css_provider = Gtk::CssProvider::create();
  css_provider->load_from_data(
      ".tipo-pill {"
      "  border-radius: 12px;"
      "  padding: 2px 8px;"
      "  margin: 1px 2px;"
      "  min-width: 60px;"
      "  font-weight: bold;"
      "  font-size: 9pt;"
      "  color: white;"
      "}"

      ".tipo-venta-ingreso {"
      "  background-color: #2ecc71;"
      "}"

      ".tipo-pago {"
      "  background-color: #3498db;"
      "}"

      ".tipo-refill {"
      "  background-color: #f39c12;"
      "}"

      ".tipo-otro {"
      "  background-color: #95a5a6;"
      "}"

      ".tipo-entrada-detalle {"
      "  background-color: #0f5f30;" // Verde más oscuro
      "}"

      ".tipo-salida-detalle {"
      "  background-color: #c0392b;" // Rojo más oscuro
      "}");

  Gtk::StyleContext::add_provider_for_display(
      Gdk::Display::get_default(),
      css_provider,
      GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
}

VMovimientos::~VMovimientos()
{
}

void VMovimientos::on_setup_id(const Glib::RefPtr<Gtk::ListItem> &list_item)
{
  auto expander = Gtk::make_managed<Gtk::TreeExpander>();
  auto label = Gtk::make_managed<Gtk::Label>();
  label->set_halign(Gtk::Align::END);
  expander->set_child(*label);
  list_item->set_child(*expander);
}

void VMovimientos::on_setup_label(const Glib::RefPtr<Gtk::ListItem> &list_item)
{
  list_item->set_child(*Gtk::make_managed<Gtk::Label>("", Gtk::Align::END));
}

void VMovimientos::on_setup_label_text(const Glib::RefPtr<Gtk::ListItem> &list_item)
{
  list_item->set_child(*Gtk::make_managed<Gtk::Label>("", Gtk::Align::START));
}

void VMovimientos::on_setup_button(const Glib::RefPtr<Gtk::ListItem> &list_item)
{
  auto button = (Gtk::make_managed<Gtk::Button>());
  button->set_css_classes({"pill"});
  button->set_hexpand(false);
  button->set_vexpand(false);
  button->set_halign(Gtk::Align::CENTER);
  button->set_halign(Gtk::Align::CENTER);
  button->set_valign(Gtk::Align::CENTER);
  list_item->set_child(*button);
}

void VMovimientos::on_bind_id(const Glib::RefPtr<Gtk::ListItem> &list_item)
{
  auto row = std::dynamic_pointer_cast<Gtk::TreeListRow>(list_item->get_item());

  auto col_log = std::dynamic_pointer_cast<MLog>(row->get_item());
  if (col_log)
  {
    auto expander = dynamic_cast<Gtk::TreeExpander *>(list_item->get_child());
    expander->set_list_row(row);
    auto label = dynamic_cast<Gtk::Label *>(expander->get_child());

    if (col_log->m_id != 0)
      label->set_text(Glib::ustring::format(col_log->m_id));
    return;
  }
}

void VMovimientos::on_bind_usuario(const Glib::RefPtr<Gtk::ListItem> &list_item)
{
  auto row = std::dynamic_pointer_cast<Gtk::TreeListRow>(list_item->get_item());
  auto col = std::dynamic_pointer_cast<MLog>(row->get_item());
  auto label = dynamic_cast<Gtk::Label *>(list_item->get_child());

  label->set_markup(col->m_user);
}

void VMovimientos::on_bind_tipo(const Glib::RefPtr<Gtk::ListItem> &list_item)
{
    auto row = std::dynamic_pointer_cast<Gtk::TreeListRow>(list_item->get_item());
    auto col = std::dynamic_pointer_cast<MLog>(row->get_item());
    auto label = dynamic_cast<Gtk::Label *>(list_item->get_child());

    if (!label || !col)
        return;

    // Determinar texto a mostrar
    std::string texto;
    if (col->m_id == 0) { // Es un detalle
        texto = col->m_tipo == "entrada" ? "Entrada" : "Salida";
    } else {
        texto = col->m_tipo;
    }

    // Configuración del label
    label->set_text(texto);
    label->set_xalign(0.5);
    label->set_justify(Gtk::Justification::CENTER);
    
    // Color según el tipo
    std::string color_class;
    if (col->m_id == 0) {
        // Para detalles
        if (texto == "Entrada")
            color_class = "tipo-entrada-detalle";
        else
            color_class = "tipo-salida-detalle";
    } else {
        // Para movimientos principales
        if (texto == "Ingreso" || texto == "Venta")
            color_class = "tipo-venta-ingreso";
        else if (texto == "Pago" || texto == "Pago Manual")
            color_class = "tipo-pago";
        else if (texto == "Refill")
            color_class = "tipo-refill";
        else
            color_class = "tipo-otro";
    }
    
    label->set_css_classes({"tipo-pill", color_class});
}

void VMovimientos::on_bind_concepto(const Glib::RefPtr<Gtk::ListItem> &list_item)
{
  auto row = std::dynamic_pointer_cast<Gtk::TreeListRow>(list_item->get_item());
  auto col = std::dynamic_pointer_cast<MLog>(row->get_item());
  auto label = dynamic_cast<Gtk::Label *>(list_item->get_child());

  label->set_text(col->m_descripcion);
}

void VMovimientos::on_bind_ingreso(const Glib::RefPtr<Gtk::ListItem> &list_item)
{
  auto row = std::dynamic_pointer_cast<Gtk::TreeListRow>(list_item->get_item());
  auto col = std::dynamic_pointer_cast<MLog>(row->get_item());
  auto label = dynamic_cast<Gtk::Label *>(list_item->get_child());

  label->set_text("$" + Glib::ustring::format(col->m_ingreso));
}

void VMovimientos::on_bind_cambio(const Glib::RefPtr<Gtk::ListItem> &list_item)
{
  auto row = std::dynamic_pointer_cast<Gtk::TreeListRow>(list_item->get_item());
  auto col = std::dynamic_pointer_cast<MLog>(row->get_item());
  auto label = dynamic_cast<Gtk::Label *>(list_item->get_child());

  label->set_text("$" + Glib::ustring::format(col->m_cambio));
}

void VMovimientos::on_bind_total(const Glib::RefPtr<Gtk::ListItem> &list_item)
{
  auto row = std::dynamic_pointer_cast<Gtk::TreeListRow>(list_item->get_item());
  auto col = std::dynamic_pointer_cast<MLog>(row->get_item());
  auto label = dynamic_cast<Gtk::Label *>(list_item->get_child());

  label->set_text("$" + Glib::ustring::format(col->m_total));
}

void VMovimientos::on_bind_estatus(const Glib::RefPtr<Gtk::ListItem> &list_item)
{
  auto row = std::dynamic_pointer_cast<Gtk::TreeListRow>(list_item->get_item());
  auto col = std::dynamic_pointer_cast<MLog>(row->get_item());
  auto label = dynamic_cast<Gtk::Label *>(list_item->get_child());

  label->set_markup(col->m_estatus);
}

void VMovimientos::on_bind_fecha(const Glib::RefPtr<Gtk::ListItem> &list_item)
{
  auto row = std::dynamic_pointer_cast<Gtk::TreeListRow>(list_item->get_item());
  auto col = std::dynamic_pointer_cast<MLog>(row->get_item());
  auto label = dynamic_cast<Gtk::Label *>(list_item->get_child());

  label->set_text(col->m_fecha.format("%A, %d de %B de %Y - Hora: %H:%M"));
}

namespace View
{
  const char *movimientos_ui = R"(<?xml version='1.0' encoding='UTF-8'?>
<!-- Created with Cambalache 0.96.1 -->
<interface>
  <!-- interface-name movimientos.ui -->
  <requires lib="gtk" version="4.0"/>
  <object class="GtkBox" id="box_movimientos">
    <property name="orientation">vertical</property>
    <property name="spacing">5</property>
    <child>
      <object class="GtkBox">
        <property name="spacing">7</property>
        <child>
          <object class="GtkButton" id="btn_printer">
            <property name="icon-name">document-print-symbolic</property>
            <property name="tooltip-text">Reimprimir Ticket</property>
            <style>
              <class name="suggested-action"/>
            </style>
          </object>
        </child>
        <child>
          <object class="GtkButton" id="btn_imprime_corte">
            <property name="halign">start</property>
            <property name="hexpand">true</property>
            <property name="icon-name">printer-network-symbolic</property>
            <property name="tooltip-text">Imprime Corte</property>
            <style>
              <class name="warning"/>
            </style>
          </object>
        </child>
        <child>
          <object class="GtkDropDown" id="dp_tipo">
            <property name="tooltip-text">Tipo de Movimiento</property>
          </object>
        </child>
        <child>
          <object class="GtkEntry" id="ety_ini">
            <property name="placeholder-text">Fecha Inicio</property>
            <property name="secondary-icon-name">x-office-calendar-symbolic</property>
          </object>
        </child>
        <child>
          <object class="GtkEntry" id="ety_fin">
            <property name="placeholder-text">Fecha Fin</property>
            <property name="secondary-icon-name">x-office-calendar-symbolic</property>
          </object>
        </child>
        <child>
          <object class="GtkBox">
            <property name="css-classes">linked</property>
            <property name="tooltip-text">Paginacion</property>
            <child>
              <object class="GtkSpinButton" id="spin_pag">
                <property name="height-request">1</property>
                <property name="numeric">true</property>
                <property name="snap-to-ticks">true</property>
              </object>
            </child>
            <child>
              <object class="GtkEntry" id="ety_pag">
                <property name="css-classes">dim-label</property>
                <property name="editable">false</property>
                <property name="max-width-chars">5</property>
                <property name="text">de 6</property>
              </object>
            </child>
          </object>
        </child>
        <child>
          <object class="GtkButton" id="btn_aplica_filtro">
            <property name="halign">end</property>
            <property name="hexpand">true</property>
            <property name="icon-name">object-select-symbolic</property>
            <property name="tooltip-text">Aplicar Filtro</property>
          </object>
        </child>
        <child>
          <object class="GtkButton" id="btn_remueve_filtros">
            <property name="halign">end</property>
            <property name="icon-name">edit-delete-symbolic</property>
            <property name="tooltip-text">Eliminar Filtros</property>
          </object>
        </child>
      </object>
    </child>
    <child>
      <object class="GtkScrolledWindow">
        <child>
          <object class="GtkColumnView" id="column_log">
            <property name="enable-rubberband">true</property>
            <property name="hexpand">true</property>
            <property name="show-column-separators">false</property>
            <property name="show-row-separators">true</property>
            <property name="vexpand">true</property>
          </object>
        </child>
      </object>
    </child>
    <child>
      <object class="GtkLabel" id="lbl_total_registros">
        <property name="css-classes">dim-label</property>
        <property name="halign">start</property>
        <property name="label">Mostrando 10 de 100 registros</property>
      </object>
    </child>
  </object>
</interface>
)";
} // namespace View