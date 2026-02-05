#include "view/configuracion/terminal.hpp"

VTerminal::VTerminal(BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &refBuilder) : Gtk::Box(cobject), m_builder(refBuilder)
{
	v_btn_add_terminal = m_builder->get_widget<Gtk::Button>("btn_agregar_terminal");
	v_flow_terminals = m_builder->get_widget<Gtk::FlowBox>("flowbox_terminales");
	v_rvl = m_builder->get_widget<Gtk::Revealer>("rvl");
	this->set_hexpand(true);
}

VTerminal::~VTerminal()
{
}

VTerminal::VFormTerminal::VFormTerminal(BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &refBuilder) : m_builder(refBuilder), Gtk::Box(cobject)
{
	v_btn_close_rvl = m_builder->get_widget<Gtk::Button>("btn_close_rvl");
	v_ety_token = m_builder->get_widget<Gtk::Entry>("ety_token");
	v_btn_verifica_token = m_builder->get_widget<Gtk::Button>("btn_verifica_token");
	v_list_terminales = m_builder->get_widget<Gtk::ListBox>("list_terminales");
	v_lbl_total_terminales = m_builder->get_widget<Gtk::Label>("lbl_total_terminales");
	v_btn_agregar = m_builder->get_widget<Gtk::Button>("btn_agregar");
	this->set_size_request(-1,400);
}

VTerminal::VFormTerminal::~VFormTerminal()
{
}


VTerminal::VFormTerminal::Row::Row(const std::string &titulo,const std::string &subtitulo)
{
	v_image_pos = Gtk::manage(new Gtk::Image());
    v_image_pos->set_from_icon_name("device-portable-sd-symbolic");
    v_image_pos->set_icon_size(Gtk::IconSize::LARGE);;

    v_titulo = Gtk::manage(new Gtk::Label(titulo));
    v_titulo->set_halign(Gtk::Align::START);
    v_titulo->set_css_classes({"title-4"});
    v_subtitulo = Gtk::manage(new Gtk::Label(subtitulo));
    v_subtitulo->set_css_classes({"dim-label"});
    v_subtitulo->set_ellipsize(Pango::EllipsizeMode::MIDDLE);
    v_subtitulo->set_justify(Gtk::Justification::LEFT);

    auto box_label = Gtk::manage(new Gtk::Box(Gtk::Orientation::VERTICAL));
    box_label->append(*v_titulo);
    box_label->append(*v_subtitulo);
    box_label->set_hexpand();

    v_box = Gtk::manage(new Gtk::Box(Gtk::Orientation::HORIZONTAL));
    v_box->append(*v_image_pos);
    v_box->append(*box_label);
    v_box->set_margin(5);

    set_child(*v_box);
}

VTerminal::VFormTerminal::Row::~Row()
{
}

namespace View
{
	namespace Conf
	{
		const char *form_terminal_ui = R"(<?xml version="1.0" encoding="UTF-8"?>
<interface>
  <requires lib="gtk" version="4.0"/>
  <object class="GtkBox" id="box_form">
    <property name="margin-bottom">5</property>
    <property name="margin-top">5</property>
    <property name="margin-start">5</property>
    <property name="margin-end">5</property>
    <property name="orientation">1</property>
    <property name="spacing">10</property>
    <child>
      <object class="GtkBox">
        <property name="orientation">0</property>
        <property name="homogeneous">false</property>
        <child>
          <object class="GtkButton" id="btn_close_rvl">
            <property name="halign">1</property>
            <property name="valign">3</property>
            <property name="icon-name">close</property>
            <style>
              <class name="circular"/>
              <class name="destructive-action"/>
            </style>
          </object>
        </child>
        <child>
          <object class="GtkLabel" id="lbl_titulo_rvl">
            <property name="hexpand">true</property>
            <property name="halign">3</property>
            <property name="valign">3</property>
            <property name="label" translatable="yes">Agregar Nueva Terminal</property>
            <style>
              <class name="title-2"/>
            </style>
          </object>
        </child>
      </object>
    </child>
    <child>
      <object class="GtkBox">
        <property name="hexpand">true</property>
        <child>
          <object class="GtkLabel">
            <property name="halign">1</property>
            <property name="label" translatable="yes">&lt;b&gt;Primera vez configurando una terminal Point?&lt;/b&gt;
Crea una aplicación en Mercado Pago y genera tu Access Token.</property>
            <property name="use-markup">true</property>
            <property name="height-request">100</property>
            <property name="wrap">true</property>
            <style>
              <class name="title-5"/>
            </style>
          </object>
        </child>
        <child>
          <object class="GtkLinkButton">
            <property name="hexpand">true</property>
            <property name="halign">2</property>
            <property name="label" translatable="yes">Crear aplicación</property>
            <property name="uri">https://www.mercadopago.com.mx/developers/es/docs/mp-point/create-application</property>
          </object>
        </child>
      </object>
    </child>
    <child>
      <object class="GtkBox">
        <property name="spacing">5</property>
        <child>
          <object class="GtkEntry" id="ety_token">
            <property name="hexpand">true</property>
            <property name="placeholder-text" translatable="yes">Access Token</property>
            <property name="truncate-multiline">true</property>
            <property name="tooltip-text" translatable="yes">Ingrese el Access Token generada en su aplicación</property>
          </object>
        </child>
        <child>
          <object class="GtkButton" id="btn_verifica_token">
            <property name="label" translatable="yes">Consultar</property>
            <style>
              <class name="suggested-action"/>
            </style>
          </object>
        </child>
      </object>
    </child>
    <child>
      <object class="GtkBox">
        <property name="orientation">1</property>
        <child>
          <object class="GtkScrolledWindow">
            <property name="hexpand">true</property>
            <property name="vexpand">true</property>
            <child>
              <object class="GtkListBox" id="list_terminales">
                <style>
                  <class name="boxed-list"/>
                  <class name="rich-list"/>
                </style>
              </object>
            </child>
          </object>
        </child>
        <child>
          <object class="GtkLabel" id="lbl_total_terminales">
            <property name="halign">1</property>
            <property name="label" translatable="yes">Total Terminales Encontradas: 0</property>
            <style>
              <class name="dim-label"/>
            </style>
          </object>
        </child>
        <child>
          <object class="GtkEntry" id="ety_alias">
            <property name="margin-top">10</property>
            <property name="placeholder-text" translatable="yes">Alias (Opcional)</property>
          </object>
        </child>
        <child>
          <object class="GtkEntry" id="ety_descripcion">
            <property name="placeholder-text" translatable="yes">Descripcion (Opcional)</property>
          </object>
        </child>
        <child>
          <object class="GtkButton" id="btn_agregar">
            <property name="margin-top">10</property>
            <property name="halign">0</property>
            <property name="hexpand">true</property>
            <property name="label" translatable="yes">Añadir terminal seleccionada</property>
            <property name="sensitive">false</property>
            <style>
              <class name="pill"/>
              <class name="suggested-action"/>
            </style>
          </object>
        </child>
      </object>
    </child>
  </object>
</interface>)";

		const char *terminal_ui = R"(<?xml version="1.0" encoding="UTF-8"?>
<interface>
  <requires lib="gtk" version="4.0"/>
  <object class="GtkBox" id="box_terminales">
    <property name="margin-bottom">16</property>
    <property name="margin-end">16</property>
    <property name="margin-start">16</property>
    <property name="margin-top">16</property>
    <property name="spacing">16</property>
    <property name="vexpand">true</property>
    <property name="hexpand">true</property>
    <child>
      <object class="GtkBox">
        <property name="orientation">1</property>
        <child>
          <object class="GtkBox">
            <property name="homogeneous">false</property>
            <property name="spacing">8</property>
            <child>
              <object class="GtkLabel" id="lbl_titulo">
                <property name="hexpand">true</property>
                <property name="halign">1</property>
                <property name="label">Terminales de Pago</property>
                <property name="valign">1</property>
                <property name="xalign">0</property>
                <style>
                  <class name="title-1"/>
                </style>
              </object>
            </child>
            <child>
              <object class="GtkButton" id="btn_agregar_terminal">
                <property name="halign">2</property>
                <property name="icon-name">add</property>
                <property name="valign">3</property>
                <style>
                  <class name="suggested-action"/>
                  <class name="circular"/>
                </style>
              </object>
            </child>
          </object>
        </child>
        <child>
          <object class="GtkScrolledWindow" id="scrolled_terminales">
            <property name="hexpand">true</property>
            <property name="vexpand">true</property>
            <property name="propagate-natural-height">true</property>
            <property name="propagate-natural-width">true</property>
            <child>
              <object class="GtkFlowBox" id="flowbox_terminales">
                <property name="hexpand">true</property>
                <property name="vexpand">true</property>
                <property name="column-spacing">16</property>
                <property name="homogeneous">true</property>
                <property name="max-children-per-line">4</property>
                <property name="min-children-per-line">1</property>
                <property name="orientation">0</property>
                <property name="row-spacing">16</property>
                <property name="selection-mode">0</property>
              </object>
            </child>
          </object>
        </child>
      </object>
    </child>
    <child>
      <object class="GtkRevealer" id="rvl">
        <property name="hexpand-set">true</property>
        <property name="transition-type">3</property>
      </object>
    </child>
  </object>
</interface>)";
	} // namespace Conf
} // namespace View