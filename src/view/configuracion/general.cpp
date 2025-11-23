#include "view/configuracion/general.hpp"

VGeneral::VGeneral(BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &refBuilder) : Gtk::Box(cobject),
                                                                                            m_builder(refBuilder)
{
    v_ety_mensaje_inicio = m_builder->get_widget<Gtk::Entry>("ety_mensaje_inicio");
    v_btn_reinicia_val = m_builder->get_widget<Gtk::Button>("btn_reinicia_val");
    v_btn_actualiza_pos = m_builder->get_widget<Gtk::Button>("btn_actualiza_pos");
    v_btn_retirada = m_builder->get_widget<Gtk::Button>("btn_retirada");
    v_btn_imagen = m_builder->get_widget<Gtk::Button>("btn_imagen");
    v_btn_imagen_2 = m_builder->get_widget<Gtk::Button>("btn_imagen_2");
    v_btn_desactiva_carrousel = m_builder->get_widget<Gtk::Button>("btn_desactiva_carrousel");
    v_chk_mostrar_notificaciones = m_builder->get_widget<Gtk::CheckButton>("chk_mostrar_notificaciones");

    v_btn_retirada->set_visible(false);
}

VGeneral::~VGeneral()
{
}

namespace View
{
    namespace Conf
    {
        const char *general_ui = R"(<?xml version='1.0' encoding='UTF-8'?>
<!-- Created with Cambalache 0.96.1 -->
<interface>
  <!-- interface-name general.ui -->
  <requires lib="gtk" version="4.0"/>
  <object class="GtkBox" id="boxConfGeneral">
    <property name="homogeneous">True</property>
    <property name="margin-bottom">10</property>
    <property name="margin-end">10</property>
    <property name="margin-start">10</property>
    <property name="margin-top">10</property>
    <property name="orientation">vertical</property>
    <child>
      <object class="GtkLabel">
        <property name="halign">center</property>
        <property name="label" translatable="yes">Configuración General</property>
        <style>
          <class name="title-1"/>
        </style>
      </object>
    </child>
    <child>
      <object class="GtkLabel">
        <property name="halign">start</property>
        <property name="label" translatable="yes">Visualización</property>
        <property name="margin-top">25</property>
        <style>
          <class name="title-2"/>
        </style>
      </object>
    </child>
    <child>
      <object class="GtkCheckButton" id="chk_mostrar_notificaciones">
        <property name="active">true</property>
        <property name="label" translatable="yes">Mostrar Notificaciónes</property>
      </object>
    </child>
    <child>
      <object class="GtkBox">
        <property name="valign">center</property>
        <child>
          <object class="GtkLabel">
            <property name="halign">start</property>
            <property name="label">Mensaje de Inicio</property>
            <property name="margin-end">10</property>
            <property name="width-request">160</property>
          </object>
        </child>
        <child>
          <object class="GtkEntry" id="ety_mensaje_inicio">
            <property name="halign">baseline-fill</property>
            <property name="hexpand">True</property>
          </object>
        </child>
      </object>
    </child>
    <child>
      <object class="GtkBox">
        <property name="valign">center</property>
        <child>
          <object class="GtkLabel">
            <property name="halign">start</property>
            <property name="label">Subir Icono POS</property>
            <property name="margin-end">10</property>
            <property name="width-request">160</property>
          </object>
        </child>
        <child>
          <object class="GtkButton" id="btn_imagen">
            <property name="focusable">False</property>
            <property name="hexpand">True</property>
            <property name="label">Seleccione Imagen</property>
          </object>
        </child>
      </object>
    </child>
    <child>
      <object class="GtkBox">
        <property name="valign">center</property>
        <child>
          <object class="GtkLabel">
            <property name="halign">start</property>
            <property name="label">Subir Carpeta Carrousel</property>
            <property name="margin-end">10</property>
            <property name="width-request">160</property>
          </object>
        </child>
        <child>
          <object class="GtkButton" id="btn_imagen_2">
            <property name="focusable">False</property>
            <property name="hexpand">True</property>
            <property name="label">Selecciona Carpeta</property>
          </object>
        </child>
      </object>
    </child>
    <child>
      <object class="GtkButton" id="btn_desactiva_carrousel">
        <property name="label">Desactivar Carrousel</property>
        <property name="valign">center</property>
        <style>
          <class name="warning"/>
        </style>
      </object>
    </child>
    <child>
      <object class="GtkSeparator">
        <property name="valign">center</property>
      </object>
    </child>
    <child>
      <object class="GtkLabel">
        <property name="halign">start</property>
        <property name="label" translatable="yes">Operaciones en efectivo</property>
        <style>
          <class name="title-2"/>
        </style>
      </object>
    </child>
    <child>
      <object class="GtkCheckButton">
        <property name="active">true</property>
        <property name="label" translatable="yes">Redondear cambio a favor del cliente</property>
      </object>
    </child>
    <child>
      <object class="GtkCheckButton">
        <property name="active">true</property>
        <property name="label" translatable="yes">Poder finalizar cualquier operación (No Disponible)</property>
        <property name="sensitive">False</property>
      </object>
    </child>
    <child>
      <object class="GtkSeparator">
        <property name="valign">center</property>
      </object>
    </child>
    <child>
      <object class="GtkLabel">
        <property name="halign">start</property>
        <property name="label" translatable="yes">Validadores</property>
        <style>
          <class name="title-2"/>
        </style>
      </object>
    </child>
    <child>
      <object class="GtkButton" id="btn_reinicia_val">
        <property name="halign">baseline</property>
        <property name="label">Reiniciar Validadores</property>
        <property name="valign">center</property>
      </object>
    </child>
    <child>
      <object class="GtkButton" id="btn_actualiza_pos">
        <property name="label">Actualiza Sistema POS</property>
        <property name="valign">center</property>
      </object>
    </child>
    <child>
      <object class="GtkButton" id="btn_retirada">
        <property name="css-classes">destructive-action</property>
        <property name="label">Retirada Cassette</property>
        <property name="valign">center</property>
      </object>
    </child>
  </object>
</interface>

)";
    } // namespace Conf
} // namespace View