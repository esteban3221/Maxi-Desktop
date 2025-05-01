#include "view/configuracion/usuarios.hpp"

VUsuarios::VUsuarios(BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &refBuilder) : Gtk::Box(cobject),


                                                                                              m_builder(refBuilder)
{
    v_treeview = m_builder->get_widget<Gtk::ColumnView>("tree_usuarios");

    v_btn_nuevo = m_builder->get_widget<Gtk::Button>("btn_new");
    v_btn_modificar = m_builder->get_widget<Gtk::Button>("btn_edit");
    v_btn_eliminar = m_builder->get_widget<Gtk::Button>("btn_del");

    for (size_t i = 0; i < 19; i++)
        v_chk_roles[i] = m_builder->get_widget<Gtk::CheckButton>("check_rol_" + std::to_string(i+1));
    v_chk_todos_roles = m_builder->get_widget<Gtk::CheckButton>("check_main");

    v_btn_guardar_roles = m_builder->get_widget<Gtk::Button>("btn_guardar_permisos");
}

VUsuarios::~VUsuarios()
{
}

void VUsuarios::on_setup_label(const Glib::RefPtr<Gtk::ListItem> &list_item)
{
    list_item->set_child(*Gtk::make_managed<Gtk::Label>("", Gtk::Align::START));
}

void VUsuarios::on_setup_row(const Glib::RefPtr<Gtk::ListItem> &list_item)
{
    auto col = std::dynamic_pointer_cast<MUsuarios>(list_item->get_item());
    auto label = dynamic_cast<Gtk::Label *>(list_item->get_child());

    label->set_text(Glib::ustring::format(col->m_usuario));
}


namespace View
{
    namespace Conf
    {
        const char *usuario_ui = R"(<?xml version='1.0' encoding='UTF-8'?>
<!-- Created with Cambalache 0.96.0 -->
<interface>
  <!-- interface-name usuarios.ui -->
  <requires lib="gtk" version="4.0"/>
  <object class="GtkBox" id="box_usuarios">
    <property name="margin-bottom">10</property>
    <property name="margin-end">10</property>
    <property name="margin-start">10</property>
    <property name="margin-top">10</property>
    <property name="spacing">10</property>
    <child>
      <object class="GtkBox">
        <property name="orientation">vertical</property>
        <child>
          <object class="GtkScrolledWindow">
            <property name="hexpand">true</property>
            <property name="vexpand">true</property>
            <property name="vexpand-set">true</property>
            <child>
              <object class="GtkColumnView" id="tree_usuarios">
                <property name="accessible-role">grid-cell</property>
                <property name="enable-rubberband">True</property>
                <property name="reorderable">False</property>
                <property name="show-row-separators">True</property>
              </object>
            </child>
          </object>
        </child>
        <child>
          <object class="GtkBox">
            <property name="css-classes">linked</property>
            <property name="margin-bottom">5</property>
            <property name="margin-end">5</property>
            <property name="margin-start">5</property>
            <property name="margin-top">5</property>
            <child>
              <object class="GtkButton" id="btn_new">
                <property name="icon-name">list-add-symbolic</property>
                <style>
                  <class name="circular"/>
                  <class name="suggested-action"/>
                </style>
              </object>
            </child>
            <child>
              <object class="GtkButton" id="btn_edit">
                <property name="icon-name">edit-symbolic</property>
                <style>
                  <class name="circular"/>
                  <class name="warning"/>
                </style>
              </object>
            </child>
            <child>
              <object class="GtkButton" id="btn_del">
                <property name="icon-name">list-remove-symbolic</property>
                <style>
                  <class name="circular"/>
                  <class name="destructive-action"/>
                </style>
              </object>
            </child>
          </object>
        </child>
      </object>
    </child>
    <child>
      <object class="GtkFrame">
        <property name="label">Permisos</property>
        <property name="label-widget">
          <object class="GtkCheckButton" id="check_main">
            <property name="label">Activar/Desactivar Todos los permisos</property>
            <property name="use-underline">True</property>
          </object>
        </property>
        <child>
          <object class="GtkGrid">
            <property name="column-homogeneous">true</property>
            <property name="margin-bottom">10</property>
            <property name="margin-end">10</property>
            <property name="margin-start">10</property>
            <property name="margin-top">10</property>
            <child>
              <object class="GtkCheckButton" id="check_rol_1">
                <property name="halign">start</property>
                <property name="label">Venta</property>
                <property name="margin-end">10</property>
                <property name="margin-start">10</property>
                <property name="valign">center</property>
                <layout>
                  <property name="column">0</property>
                  <property name="row">0</property>
                </layout>
              </object>
            </child>
            <child>
              <object class="GtkCheckButton" id="check_rol_2">
                <property name="halign">start</property>
                <property name="label">Pago</property>
                <property name="margin-end">10</property>
                <property name="margin-start">10</property>
                <property name="valign">center</property>
                <layout>
                  <property name="column">0</property>
                  <property name="row">1</property>
                </layout>
              </object>
            </child>
            <child>
              <object class="GtkCheckButton" id="check_rol_3">
                <property name="halign">start</property>
                <property name="label">Carga</property>
                <property name="margin-end">10</property>
                <property name="margin-start">10</property>
                <property name="valign">center</property>
                <layout>
                  <property name="column">0</property>
                  <property name="row">2</property>
                </layout>
              </object>
            </child>
            <child>
              <object class="GtkCheckButton" id="check_rol_4">
                <property name="halign">start</property>
                <property name="label">Retirada</property>
                <property name="margin-end">10</property>
                <property name="margin-start">10</property>
                <property name="valign">center</property>
                <layout>
                  <property name="column">0</property>
                  <property name="row">3</property>
                </layout>
              </object>
            </child>
            <child>
              <object class="GtkCheckButton" id="check_rol_5">
                <property name="halign">start</property>
                <property name="label">Cambio Manual</property>
                <property name="margin-end">10</property>
                <property name="margin-start">10</property>
                <property name="valign">center</property>
                <layout>
                  <property name="column">0</property>
                  <property name="row">4</property>
                </layout>
              </object>
            </child>
            <child>
              <object class="GtkCheckButton" id="check_rol_6">
                <property name="halign">start</property>
                <property name="label">Cambio Automatico</property>
                <property name="margin-end">10</property>
                <property name="margin-start">10</property>
                <property name="valign">center</property>
                <layout>
                  <property name="column">0</property>
                  <property name="row">5</property>
                </layout>
              </object>
            </child>
            <child>
              <object class="GtkCheckButton" id="check_rol_7">
                <property name="halign">start</property>
                <property name="label">Ingresos</property>
                <property name="margin-end">10</property>
                <property name="margin-start">10</property>
                <property name="valign">center</property>
                <layout>
                  <property name="column">0</property>
                  <property name="row">6</property>
                </layout>
              </object>
            </child>
            <child>
              <object class="GtkCheckButton" id="check_rol_8">
                <property name="halign">start</property>
                <property name="label">Enviar a casette</property>
                <property name="margin-end">10</property>
                <property name="margin-start">10</property>
                <property name="valign">center</property>
                <layout>
                  <property name="column">1</property>
                  <property name="row">0</property>
                </layout>
              </object>
            </child>
            <child>
              <object class="GtkCheckButton" id="check_rol_9">
                <property name="halign">start</property>
                <property name="label">Retirada Casette</property>
                <property name="margin-end">10</property>
                <property name="margin-start">10</property>
                <property name="valign">center</property>
                <layout>
                  <property name="column">1</property>
                  <property name="row">1</property>
                </layout>
              </object>
            </child>
            <child>
              <object class="GtkCheckButton" id="check_rol_10">
                <property name="halign">start</property>
                <property name="label">Consulta de Efectivo</property>
                <property name="margin-end">10</property>
                <property name="margin-start">10</property>
                <property name="valign">center</property>
                <layout>
                  <property name="column">1</property>
                  <property name="row">2</property>
                </layout>
              </object>
            </child>
            <child>
              <object class="GtkCheckButton" id="check_rol_11">
                <property name="halign">start</property>
                <property name="label">Mov. Pendientes</property>
                <property name="margin-end">10</property>
                <property name="margin-start">10</property>
                <property name="valign">center</property>
                <layout>
                  <property name="column">1</property>
                  <property name="row">3</property>
                </layout>
              </object>
            </child>
            <child>
              <object class="GtkCheckButton" id="check_rol_12">
                <property name="halign">start</property>
                <property name="label">Consulta Movimiento</property>
                <property name="margin-end">10</property>
                <property name="margin-start">10</property>
                <property name="valign">center</property>
                <layout>
                  <property name="column">1</property>
                  <property name="row">4</property>
                </layout>
              </object>
            </child>
            <child>
              <object class="GtkCheckButton" id="check_rol_13">
                <property name="halign">start</property>
                <property name="label">Cierre con Faltantes</property>
                <property name="margin-end">10</property>
                <property name="margin-start">10</property>
                <property name="valign">center</property>
                <layout>
                  <property name="column">1</property>
                  <property name="row">5</property>
                </layout>
              </object>
            </child>
            <child>
              <object class="GtkSeparator">
                <layout>
                  <property name="column">0</property>
                  <property name="column-span">2</property>
                  <property name="row">8</property>
                </layout>
              </object>
            </child>
            <child>
              <object class="GtkCheckButton" id="check_rol_14">
                <property name="halign">start</property>
                <property name="label">Estadisticas</property>
                <property name="margin-end">10</property>
                <property name="margin-start">10</property>
                <property name="valign">center</property>
                <layout>
                  <property name="column">0</property>
                  <property name="row">9</property>
                </layout>
              </object>
            </child>
            <child>
              <object class="GtkCheckButton" id="check_rol_15">
                <property name="halign">start</property>
                <property name="label">Fianza</property>
                <property name="margin-end">10</property>
                <property name="margin-start">10</property>
                <property name="valign">center</property>
                <layout>
                  <property name="column">0</property>
                  <property name="row">10</property>
                </layout>
              </object>
            </child>
            <child>
              <object class="GtkCheckButton" id="check_rol_16">
                <property name="halign">start</property>
                <property name="label">Mostrar Reportes</property>
                <property name="margin-end">10</property>
                <property name="margin-start">10</property>
                <property name="valign">center</property>
                <layout>
                  <property name="column">0</property>
                  <property name="row">11</property>
                </layout>
              </object>
            </child>
            <child>
              <object class="GtkCheckButton" id="check_rol_17">
                <property name="halign">start</property>
                <property name="label">Configuracion</property>
                <property name="margin-end">10</property>
                <property name="margin-start">10</property>
                <property name="valign">center</property>
                <layout>
                  <property name="column">1</property>
                  <property name="row">9</property>
                </layout>
              </object>
            </child>
            <child>
              <object class="GtkCheckButton" id="check_rol_18">
                <property name="halign">start</property>
                <property name="label">Salir a Windows</property>
                <property name="margin-end">10</property>
                <property name="margin-start">10</property>
                <property name="valign">center</property>
                <layout>
                  <property name="column">1</property>
                  <property name="row">10</property>
                </layout>
              </object>
            </child>
            <child>
              <object class="GtkCheckButton" id="check_rol_19">
                <property name="halign">start</property>
                <property name="label">Apagar el equipo</property>
                <property name="margin-end">10</property>
                <property name="margin-start">10</property>
                <property name="valign">center</property>
                <layout>
                  <property name="column">1</property>
                  <property name="row">11</property>
                </layout>
              </object>
            </child>
            <child>
              <object class="GtkButton" id="btn_guardar_permisos">
                <property name="halign">center</property>
                <property name="label">Guardar</property>
                <property name="margin-top">50</property>
                <property name="valign">center</property>
                <style>
                  <class name="suggested-action"/>
                  <class name="pill"/>
                </style>
                <layout>
                  <property name="column">0</property>
                  <property name="column-span">2</property>
                  <property name="row">15</property>
                </layout>
              </object>
            </child>
          </object>
        </child>
      </object>
    </child>
  </object>
</interface>
)";
    } // namespace Conf
} // namespace View