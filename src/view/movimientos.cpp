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
}

VMovimientos::~VMovimientos()
{
}

void VMovimientos::on_setup_label(const Glib::RefPtr<Gtk::ListItem> &list_item)
{
    list_item->set_child(*Gtk::make_managed<Gtk::Label>("", Gtk::Align::END));
}

void VMovimientos::on_setup_label_text(const Glib::RefPtr<Gtk::ListItem> &list_item)
{
    list_item->set_child(*Gtk::make_managed<Gtk::Label>("", Gtk::Align::START));
}

void VMovimientos::on_bind_id(const Glib::RefPtr<Gtk::ListItem> &list_item)
{
    auto col = std::dynamic_pointer_cast<MLog>(list_item->get_item());
    auto label = dynamic_cast<Gtk::Label *>(list_item->get_child());

    label->set_text(Glib::ustring::format(col->m_id));
}

void VMovimientos::on_bind_usuario(const Glib::RefPtr<Gtk::ListItem> &list_item)
{
    auto col = std::dynamic_pointer_cast<MLog>(list_item->get_item());
    auto label = dynamic_cast<Gtk::Label *>(list_item->get_child());

    label->set_markup(col->m_user);
}

void VMovimientos::on_bind_tipo(const Glib::RefPtr<Gtk::ListItem> &list_item)
{
    auto col = std::dynamic_pointer_cast<MLog>(list_item->get_item());
    auto label = dynamic_cast<Gtk::Label *>(list_item->get_child());

    label->set_text(Glib::ustring::format(col->m_tipo));
}

void VMovimientos::on_bind_ingreso(const Glib::RefPtr<Gtk::ListItem> &list_item)
{
    auto col = std::dynamic_pointer_cast<MLog>(list_item->get_item());
    auto label = dynamic_cast<Gtk::Label *>(list_item->get_child());

    label->set_text(Glib::ustring::format(col->m_ingreso));
}

void VMovimientos::on_bind_cambio(const Glib::RefPtr<Gtk::ListItem> &list_item)
{
    auto col = std::dynamic_pointer_cast<MLog>(list_item->get_item());
    auto label = dynamic_cast<Gtk::Label *>(list_item->get_child());

    label->set_text(Glib::ustring::format(col->m_cambio));
}

void VMovimientos::on_bind_total(const Glib::RefPtr<Gtk::ListItem> &list_item)
{
    auto col = std::dynamic_pointer_cast<MLog>(list_item->get_item());
    auto label = dynamic_cast<Gtk::Label *>(list_item->get_child());

    label->set_text(Glib::ustring::format(col->m_total));
}

void VMovimientos::on_bind_estatus(const Glib::RefPtr<Gtk::ListItem> &list_item)
{
    auto col = std::dynamic_pointer_cast<MLog>(list_item->get_item());
    auto label = dynamic_cast<Gtk::Label *>(list_item->get_child());

    label->set_markup(col->m_estatus);
}

void VMovimientos::on_bind_fecha(const Glib::RefPtr<Gtk::ListItem> &list_item)
{
    auto col = std::dynamic_pointer_cast<MLog>(list_item->get_item());
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
            <property name="show-column-separators">true</property>
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