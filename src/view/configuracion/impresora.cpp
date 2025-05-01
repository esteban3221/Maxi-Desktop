#include "view/configuracion/impresora.hpp"

VImpresora::VImpresora(BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &refBuilder) : Gtk::Box(cobject),m_builder(refBuilder)
{
    v_list_box_print = m_builder->get_widget<Gtk::ListBox>("list_box_print");
    v_ntbook_propiedades = m_builder->get_widget<Gtk::Notebook>("ntbookPropiedades");
    v_text_view = m_builder->get_widget<Gtk::TextView>("text_view");

    local.v_lbox_activa = m_builder->get_widget<Gtk::ListBox>("listBoxPage1_");
    local.v_switch_activa = m_builder->get_widget<Gtk::Switch>("switchImpresion_");
    local.v_lbox_vizualizador = m_builder->get_widget<Gtk::ListBox>("listBoxVisualizacion1_");

    for (size_t i = 0; i < 6; i++)
    {
        local.v_check_vizualizador[i] = m_builder->get_widget<Gtk::CheckButton>("checkConfig" + std::to_string(i+1) + "_");
        remoto.v_check_vizualizador[i] = m_builder->get_widget<Gtk::CheckButton>("checkConfig" + std::to_string(i+1));
    }

    local.v_lbox_test = m_builder->get_widget<Gtk::ListBox>("listBoxTest1_");

    remoto.v_lbox_activa = m_builder->get_widget<Gtk::ListBox>("listBoxPage1");
    remoto.v_switch_activa = m_builder->get_widget<Gtk::Switch>("switchImpresion");
    remoto.v_lbox_vizualizador = m_builder->get_widget<Gtk::ListBox>("listBoxVisualizacion1");
    remoto.v_lbox_test = m_builder->get_widget<Gtk::ListBox>("listBoxTest1");
}

VImpresora::~VImpresora()
{
}

VImpresora::VListPrinters::VListPrinters(const std::string &titulo, const std::string &subtitulo)
{
    v_image_printer = Gtk::manage(new Gtk::Image());
    v_image_printer->set_from_icon_name("document-print-symbolic");
    v_image_printer->set_icon_size(Gtk::IconSize::LARGE);

    v_image_check = Gtk::manage(new Gtk::Image());
    v_image_check->set_from_icon_name("checkmark-symbolic");
    v_image_check->set_opacity(0);

    v_titulo = Gtk::manage(new Gtk::Label(titulo));
    v_titulo->set_halign(Gtk::Align::START);
    v_titulo->set_css_classes({"title-4"});
    v_subtitulo = Gtk::manage(new Gtk::Label(subtitulo));
    v_subtitulo->set_css_classes({"dim-label"});
    v_subtitulo->set_ellipsize(Pango::EllipsizeMode::END);
    v_subtitulo->set_justify(Gtk::Justification::LEFT);

    auto box_label = Gtk::manage(new Gtk::Box(Gtk::Orientation::VERTICAL));
    box_label->append(*v_titulo);
    box_label->append(*v_subtitulo);
    box_label->set_hexpand();

    v_box = Gtk::manage(new Gtk::Box(Gtk::Orientation::HORIZONTAL));
    v_box->append(*v_image_printer);
    v_box->append(*box_label);
    v_box->append(*v_image_check);
    v_image_check->set_halign(Gtk::Align::END);
    v_box->set_margin(5);

    set_child(*v_box);
}

namespace View
{
    namespace Conf
    {
        const char *impresora_ui = R"(<?xml version='1.0' encoding='UTF-8'?>
<!-- Created with Cambalache 0.96.0 -->
<interface>
  <!-- interface-name impresora.ui -->
  <requires lib="gtk" version="4.0"/>
  <object class="GtkBox" id="box_config_impresora">
    <property name="homogeneous">True</property>
    <property name="margin-bottom">10</property>
    <property name="margin-end">10</property>
    <property name="margin-start">10</property>
    <property name="margin-top">10</property>
    <property name="spacing">10</property>
    <child>
      <object class="GtkBox">
        <property name="orientation">vertical</property>
        <property name="vexpand">True</property>
        <child>
          <object class="GtkLabel">
            <property name="halign">start</property>
            <property name="label">Impresoras</property>
            <property name="margin-bottom">20</property>
            <style>
              <class name="title-1"/>
            </style>
          </object>
        </child>
        <child>
          <object class="GtkLabel">
            <property name="halign">start</property>
            <property name="label">Seleccione la impresora a predeterminar para las operaciones.</property>
            <property name="lines">2</property>
            <property name="margin-bottom">10</property>
            <property name="natural-wrap-mode">word</property>
            <property name="wrap">True</property>
            <style>
              <class name="dim-label"/>
            </style>
          </object>
        </child>
        <child>
          <object class="GtkScrolledWindow">
            <property name="hscrollbar-policy">never</property>
            <property name="vexpand">True</property>
            <child>
              <object class="GtkListBox" id="list_box_print">
                <property name="selection-mode">none</property>
                <property name="show-separators">True</property>
                <child>
                  <object class="GtkListBoxRow" id="listBoxRowPrint">
                    <property name="activatable">false</property>
                    <child>
                      <object class="GtkLabel">
                        <property name="label">Impresoras no encontradas</property>
                        <style>
                          <class name="title-3"/>
                        </style>
                      </object>
                    </child>
                  </object>
                </child>
                <style>
                  <class name="rich-list"/>
                  <class name="boxed-list"/>
                </style>
              </object>
            </child>
          </object>
        </child>
      </object>
    </child>
    <child>
      <object class="GtkBox">
        <property name="orientation">vertical</property>
        <property name="vexpand">true</property>
        <child>
          <object class="GtkLabel">
            <property name="halign">start</property>
            <property name="label">Propiedades</property>
            <property name="margin-bottom">20</property>
            <style>
              <class name="title-1"/>
            </style>
          </object>
        </child>
        <child>
          <object class="GtkNotebook" id="ntbookPropiedades">
            <property name="accessible-role">none</property>
            <property name="page">0</property>
            <property name="scrollable">True</property>
            <property name="vexpand">True</property>
            <child>
              <object class="GtkNotebookPage">
                <property name="child">
                  <object class="GtkScrolledWindow">
                    <property name="hscrollbar-policy">never</property>
                    <property name="margin-bottom">10</property>
                    <property name="margin-end">10</property>
                    <property name="margin-start">10</property>
                    <property name="margin-top">10</property>
                    <child>
                      <object class="GtkBox">
                        <property name="margin-bottom">30</property>
                        <property name="margin-end">10</property>
                        <property name="margin-start">10</property>
                        <property name="orientation">vertical</property>
                        <child>
                          <object class="GtkLabel">
                            <property name="label">Activar</property>
                            <property name="margin-bottom">10</property>
                            <property name="xalign">0</property>
                            <style>
                              <class name="title-2"/>
                            </style>
                          </object>
                        </child>
                        <child>
                          <object class="GtkListBox" id="listBoxPage1_">
                            <property name="margin-bottom">30</property>
                            <property name="selection-mode">none</property>
                            <child>
                              <object class="GtkListBoxRow">
                                <child>
                                  <object class="GtkBox">
                                    <child>
                                      <object class="GtkLabel">
                                        <property name="halign">start</property>
                                        <property name="hexpand">true</property>
                                        <property name="label">Activar Impresión</property>
                                        <property name="valign">center</property>
                                        <property name="xalign">0</property>
                                      </object>
                                    </child>
                                    <child>
                                      <object class="GtkSwitch" id="switchImpresion_">
                                        <property name="halign">end</property>
                                        <property name="valign">center</property>
                                      </object>
                                    </child>
                                  </object>
                                </child>
                              </object>
                            </child>
                            <style>
                              <class name="rich-list"/>
                              <class name="boxed-list"/>
                            </style>
                          </object>
                        </child>
                        <child>
                          <object class="GtkLabel">
                            <property name="label">Visualización</property>
                            <property name="margin-bottom">10</property>
                            <property name="xalign">0</property>
                            <style>
                              <class name="title-2"/>
                            </style>
                          </object>
                        </child>
                        <child>
                          <object class="GtkListBox" id="listBoxVisualizacion1_">
                            <property name="selection-mode">none</property>
                            <child>
                              <object class="GtkListBoxRow">
                                <child>
                                  <object class="GtkBox">
                                    <child>
                                      <object class="GtkLabel">
                                        <property name="halign">start</property>
                                        <property name="hexpand">true</property>
                                        <property name="label">Mensaje de Agradecimiento</property>
                                        <property name="valign">center</property>
                                        <property name="xalign">0</property>
                                      </object>
                                    </child>
                                    <child>
                                      <object class="GtkCheckButton" id="checkConfig1_">
                                        <property name="active">true</property>
                                        <property name="halign">end</property>
                                        <property name="margin-end">10</property>
                                        <property name="margin-start">10</property>
                                        <property name="valign">center</property>
                                      </object>
                                    </child>
                                  </object>
                                </child>
                              </object>
                            </child>
                            <child>
                              <object class="GtkListBoxRow">
                                <child>
                                  <object class="GtkBox">
                                    <child>
                                      <object class="GtkLabel">
                                        <property name="halign">start</property>
                                        <property name="hexpand">true</property>
                                        <property name="label">Fecha</property>
                                        <property name="valign">center</property>
                                        <property name="xalign">0</property>
                                      </object>
                                    </child>
                                    <child>
                                      <object class="GtkCheckButton" id="checkConfig2_">
                                        <property name="active">true</property>
                                        <property name="halign">end</property>
                                        <property name="margin-end">10</property>
                                        <property name="margin-start">10</property>
                                        <property name="valign">center</property>
                                      </object>
                                    </child>
                                  </object>
                                </child>
                              </object>
                            </child>
                            <child>
                              <object class="GtkListBoxRow">
                                <child>
                                  <object class="GtkBox">
                                    <child>
                                      <object class="GtkLabel">
                                        <property name="halign">start</property>
                                        <property name="hexpand">true</property>
                                        <property name="label">Direccion</property>
                                        <property name="valign">center</property>
                                        <property name="xalign">0</property>
                                      </object>
                                    </child>
                                    <child>
                                      <object class="GtkCheckButton" id="checkConfig3_">
                                        <property name="active">true</property>
                                        <property name="halign">end</property>
                                        <property name="margin-end">10</property>
                                        <property name="margin-start">10</property>
                                        <property name="valign">center</property>
                                      </object>
                                    </child>
                                  </object>
                                </child>
                              </object>
                            </child>
                            <child>
                              <object class="GtkListBoxRow">
                                <child>
                                  <object class="GtkBox">
                                    <child>
                                      <object class="GtkLabel">
                                        <property name="halign">start</property>
                                        <property name="hexpand">true</property>
                                        <property name="label">RFC</property>
                                        <property name="valign">center</property>
                                        <property name="xalign">0</property>
                                      </object>
                                    </child>
                                    <child>
                                      <object class="GtkCheckButton" id="checkConfig4_">
                                        <property name="active">true</property>
                                        <property name="halign">end</property>
                                        <property name="margin-end">10</property>
                                        <property name="margin-start">10</property>
                                        <property name="valign">center</property>
                                      </object>
                                    </child>
                                  </object>
                                </child>
                              </object>
                            </child>
                            <child>
                              <object class="GtkListBoxRow">
                                <child>
                                  <object class="GtkBox">
                                    <child>
                                      <object class="GtkLabel">
                                        <property name="halign">start</property>
                                        <property name="hexpand">true</property>
                                        <property name="label">Vendedor</property>
                                        <property name="valign">center</property>
                                        <property name="xalign">0</property>
                                      </object>
                                    </child>
                                    <child>
                                      <object class="GtkCheckButton" id="checkConfig5_">
                                        <property name="active">true</property>
                                        <property name="halign">end</property>
                                        <property name="margin-end">10</property>
                                        <property name="margin-start">10</property>
                                        <property name="valign">center</property>
                                      </object>
                                    </child>
                                  </object>
                                </child>
                              </object>
                            </child>
                            <child>
                              <object class="GtkListBoxRow">
                                <child>
                                  <object class="GtkBox">
                                    <child>
                                      <object class="GtkLabel">
                                        <property name="halign">start</property>
                                        <property name="hexpand">true</property>
                                        <property name="label">Contacto</property>
                                        <property name="valign">center</property>
                                        <property name="xalign">0</property>
                                      </object>
                                    </child>
                                    <child>
                                      <object class="GtkCheckButton" id="checkConfig6_">
                                        <property name="active">true</property>
                                        <property name="halign">end</property>
                                        <property name="margin-end">10</property>
                                        <property name="margin-start">10</property>
                                        <property name="valign">center</property>
                                      </object>
                                    </child>
                                  </object>
                                </child>
                              </object>
                            </child>
                            <style>
                              <class name="rich-list"/>
                              <class name="boxed-list"/>
                            </style>
                          </object>
                        </child>
                        <child>
                          <object class="GtkLabel">
                            <property name="label">Test</property>
                            <property name="margin-bottom">10</property>
                            <property name="margin-top">30</property>
                            <property name="xalign">0</property>
                            <style>
                              <class name="title-2"/>
                            </style>
                          </object>
                        </child>
                        <child>
                          <object class="GtkListBox" id="listBoxTest1_">
                            <property name="selection-mode">none</property>
                            <child>
                              <object class="GtkListBoxRow">
                                <child>
                                  <object class="GtkBox">
                                    <child>
                                      <object class="GtkLabel">
                                        <property name="halign">start</property>
                                        <property name="hexpand">true</property>
                                        <property name="label">Impresion de Prueba</property>
                                        <property name="valign">center</property>
                                        <property name="xalign">0</property>
                                      </object>
                                    </child>
                                    <child>
                                      <object class="GtkImage">
                                        <property name="halign">end</property>
                                        <property name="icon-name">printer-symbolic</property>
                                      </object>
                                    </child>
                                  </object>
                                </child>
                              </object>
                            </child>
                            <style>
                              <class name="rich-list"/>
                              <class name="boxed-list"/>
                            </style>
                          </object>
                        </child>
                      </object>
                    </child>
                  </object>
                </property>
                <property name="detachable">True</property>
                <property name="reorderable">True</property>
                <property name="tab-expand">True</property>
                <property name="tab-fill">False</property>
                <property name="tab-label">Local</property>
              </object>
            </child>
            <child>
              <object class="GtkNotebookPage">
                <property name="child">
                  <object class="GtkScrolledWindow">
                    <property name="hscrollbar-policy">never</property>
                    <property name="margin-bottom">10</property>
                    <property name="margin-end">10</property>
                    <property name="margin-start">10</property>
                    <property name="margin-top">10</property>
                    <child>
                      <object class="GtkBox">
                        <property name="margin-bottom">30</property>
                        <property name="margin-end">10</property>
                        <property name="margin-start">10</property>
                        <property name="orientation">vertical</property>
                        <child>
                          <object class="GtkLabel">
                            <property name="label">Activar</property>
                            <property name="margin-bottom">10</property>
                            <property name="xalign">0</property>
                            <style>
                              <class name="title-2"/>
                            </style>
                          </object>
                        </child>
                        <child>
                          <object class="GtkListBox" id="listBoxPage1">
                            <property name="margin-bottom">30</property>
                            <property name="selection-mode">none</property>
                            <child>
                              <object class="GtkListBoxRow">
                                <child>
                                  <object class="GtkBox">
                                    <child>
                                      <object class="GtkLabel" id="lblActivar1">
                                        <property name="halign">start</property>
                                        <property name="hexpand">true</property>
                                        <property name="label">Activar Impresión</property>
                                        <property name="valign">center</property>
                                        <property name="xalign">0</property>
                                      </object>
                                    </child>
                                    <child>
                                      <object class="GtkSwitch" id="switchImpresion">
                                        <property name="halign">end</property>
                                        <property name="valign">center</property>
                                      </object>
                                    </child>
                                  </object>
                                </child>
                              </object>
                            </child>
                            <style>
                              <class name="rich-list"/>
                              <class name="boxed-list"/>
                            </style>
                          </object>
                        </child>
                        <child>
                          <object class="GtkLabel">
                            <property name="label">Visualización</property>
                            <property name="margin-bottom">10</property>
                            <property name="xalign">0</property>
                            <style>
                              <class name="title-2"/>
                            </style>
                          </object>
                        </child>
                        <child>
                          <object class="GtkListBox" id="listBoxVisualizacion1">
                            <property name="selection-mode">none</property>
                            <child>
                              <object class="GtkListBoxRow">
                                <child>
                                  <object class="GtkBox">
                                    <child>
                                      <object class="GtkLabel" id="lblMensaje1">
                                        <property name="halign">start</property>
                                        <property name="hexpand">true</property>
                                        <property name="label">Mensaje de Agradecimiento</property>
                                        <property name="valign">center</property>
                                        <property name="xalign">0</property>
                                      </object>
                                    </child>
                                    <child>
                                      <object class="GtkCheckButton" id="checkConfig1">
                                        <property name="active">true</property>
                                        <property name="halign">end</property>
                                        <property name="margin-end">10</property>
                                        <property name="margin-start">10</property>
                                        <property name="valign">center</property>
                                      </object>
                                    </child>
                                  </object>
                                </child>
                              </object>
                            </child>
                            <child>
                              <object class="GtkListBoxRow">
                                <child>
                                  <object class="GtkBox">
                                    <child>
                                      <object class="GtkLabel" id="lblFecha1">
                                        <property name="halign">start</property>
                                        <property name="hexpand">true</property>
                                        <property name="label">Fecha</property>
                                        <property name="valign">center</property>
                                        <property name="xalign">0</property>
                                      </object>
                                    </child>
                                    <child>
                                      <object class="GtkCheckButton" id="checkConfig2">
                                        <property name="active">true</property>
                                        <property name="halign">end</property>
                                        <property name="margin-end">10</property>
                                        <property name="margin-start">10</property>
                                        <property name="valign">center</property>
                                      </object>
                                    </child>
                                  </object>
                                </child>
                              </object>
                            </child>
                            <child>
                              <object class="GtkListBoxRow">
                                <child>
                                  <object class="GtkBox">
                                    <child>
                                      <object class="GtkLabel" id="lblDireccion1">
                                        <property name="halign">start</property>
                                        <property name="hexpand">true</property>
                                        <property name="label">Direccion</property>
                                        <property name="valign">center</property>
                                        <property name="xalign">0</property>
                                      </object>
                                    </child>
                                    <child>
                                      <object class="GtkCheckButton" id="checkConfig3">
                                        <property name="active">true</property>
                                        <property name="halign">end</property>
                                        <property name="margin-end">10</property>
                                        <property name="margin-start">10</property>
                                        <property name="valign">center</property>
                                      </object>
                                    </child>
                                  </object>
                                </child>
                              </object>
                            </child>
                            <child>
                              <object class="GtkListBoxRow">
                                <child>
                                  <object class="GtkBox">
                                    <child>
                                      <object class="GtkLabel" id="lblRFC1">
                                        <property name="halign">start</property>
                                        <property name="hexpand">true</property>
                                        <property name="label">RFC</property>
                                        <property name="valign">center</property>
                                        <property name="xalign">0</property>
                                      </object>
                                    </child>
                                    <child>
                                      <object class="GtkCheckButton" id="checkConfig4">
                                        <property name="active">true</property>
                                        <property name="halign">end</property>
                                        <property name="margin-end">10</property>
                                        <property name="margin-start">10</property>
                                        <property name="valign">center</property>
                                      </object>
                                    </child>
                                  </object>
                                </child>
                              </object>
                            </child>
                            <child>
                              <object class="GtkListBoxRow">
                                <child>
                                  <object class="GtkBox">
                                    <child>
                                      <object class="GtkLabel" id="lblVendedor1">
                                        <property name="halign">start</property>
                                        <property name="hexpand">true</property>
                                        <property name="label">Vendedor</property>
                                        <property name="valign">center</property>
                                        <property name="xalign">0</property>
                                      </object>
                                    </child>
                                    <child>
                                      <object class="GtkCheckButton" id="checkConfig5">
                                        <property name="active">true</property>
                                        <property name="halign">end</property>
                                        <property name="margin-end">10</property>
                                        <property name="margin-start">10</property>
                                        <property name="valign">center</property>
                                      </object>
                                    </child>
                                  </object>
                                </child>
                              </object>
                            </child>
                            <child>
                              <object class="GtkListBoxRow">
                                <child>
                                  <object class="GtkBox">
                                    <child>
                                      <object class="GtkLabel" id="lblContacto1">
                                        <property name="halign">start</property>
                                        <property name="hexpand">true</property>
                                        <property name="label">Contacto</property>
                                        <property name="valign">center</property>
                                        <property name="xalign">0</property>
                                      </object>
                                    </child>
                                    <child>
                                      <object class="GtkCheckButton" id="checkConfig6">
                                        <property name="active">true</property>
                                        <property name="halign">end</property>
                                        <property name="margin-end">10</property>
                                        <property name="margin-start">10</property>
                                        <property name="valign">center</property>
                                      </object>
                                    </child>
                                  </object>
                                </child>
                              </object>
                            </child>
                            <style>
                              <class name="rich-list"/>
                              <class name="boxed-list"/>
                            </style>
                          </object>
                        </child>
                        <child>
                          <object class="GtkListBox" id="listBoxTest1">
                            <property name="margin-top">30</property>
                            <property name="selection-mode">none</property>
                            <child>
                              <object class="GtkListBoxRow">
                                <child>
                                  <object class="GtkBox">
                                    <child>
                                      <object class="GtkLabel" id="lblImpresionPrueba1">
                                        <property name="halign">start</property>
                                        <property name="hexpand">true</property>
                                        <property name="label">Guardar</property>
                                        <property name="valign">center</property>
                                        <property name="xalign">0</property>
                                      </object>
                                    </child>
                                    <child>
                                      <object class="GtkImage">
                                        <property name="halign">end</property>
                                        <property name="icon-name">document-save-all</property>
                                      </object>
                                    </child>
                                  </object>
                                </child>
                              </object>
                            </child>
                            <style>
                              <class name="rich-list"/>
                              <class name="boxed-list"/>
                            </style>
                          </object>
                        </child>
                      </object>
                    </child>
                  </object>
                </property>
                <property name="detachable">True</property>
                <property name="reorderable">True</property>
                <property name="tab-expand">True</property>
                <property name="tab-fill">False</property>
                <property name="tab-label">Remoto</property>
              </object>
            </child>
            <style>
              <class name="tab"/>
            </style>
          </object>
        </child>
      </object>
    </child>
    <child>
      <object class="GtkBox">
        <property name="orientation">vertical</property>
        <child>
          <object class="GtkLabel">
            <property name="halign">start</property>
            <property name="label">Visualización</property>
            <property name="margin-bottom">20</property>
            <style>
              <class name="title-1"/>
            </style>
          </object>
        </child>
        <child>
          <object class="GtkScrolledWindow">
            <child>
              <object class="GtkTextView" id="text_view">
                <property name="buffer">
                  <object class="GtkTextBuffer">
                    <property name="tag-table">
                      <object class="GtkTextTagTable"/>
                    </property>
                  </object>
                </property>
                <property name="editable">False</property>
                <property name="hexpand">True</property>
                <property name="hscroll-policy">natural</property>
                <property name="left-margin">5</property>
                <property name="monospace">True</property>
                <property name="vexpand">True</property>
                <property name="vscroll-policy">natural</property>
                <property name="wrap-mode">word</property>
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