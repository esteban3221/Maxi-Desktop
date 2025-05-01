#include "view/configuracion/sistema.hpp"

VSistema::VSistema(BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &refBuilder) : Gtk::Box(cobject),
                                                                                            m_builder(refBuilder)
{
    
    v_lbl_sistema[0] = m_builder->get_widget<Gtk::EditableLabel>("ety_info_host");
    v_lbl_sistema[1] = m_builder->get_widget<Gtk::EditableLabel>("ety_info_modelo");
    v_lbl_sistema[2] = m_builder->get_widget<Gtk::EditableLabel>("ety_info_procesador");
    v_lbl_sistema[3] = m_builder->get_widget<Gtk::EditableLabel>("ety_info_memoria");
    v_lbl_sistema[4] = m_builder->get_widget<Gtk::EditableLabel>("ety_info_disco");
    
    v_list_acciones = m_builder->get_widget<Gtk::ListBox>("list_acciones");

    v_lbl_bill[0] = m_builder->get_widget<Gtk::Label>("lbl_data_");
    v_lbl_coin[0] = m_builder->get_widget<Gtk::Label>("lbl_data__");

    v_lbl_titulo_bill = m_builder->get_widget<Gtk::Label>("lbl_titulo");
    v_lbl_titulo_coin = m_builder->get_widget<Gtk::Label>("lbl_titulo_");
    for (size_t i = 0; i < 11; i++)
    {
        v_lbl_bill[i+1] = m_builder->get_widget<Gtk::Label>("lbl_data_" + std::to_string(i));
        v_lbl_coin[i+1] = m_builder->get_widget<Gtk::Label>("lbl_data_" + std::to_string(i) + "_");
    }
    
}

VSistema::~VSistema()
{
}


namespace View
{
    namespace Conf
    {
        const char *sistema_ui = R"(<?xml version='1.0' encoding='UTF-8'?>
<!-- Created with Cambalache 0.96.0 -->
<interface>
  <!-- interface-name detalle_sistema.ui -->
  <requires lib="gtk" version="4.0"/>
  <object class="GtkBox" id="infSistemas">
    <property name="margin-bottom">10</property>
    <property name="margin-end">10</property>
    <property name="margin-start">10</property>
    <property name="margin-top">10</property>
    <property name="orientation">vertical</property>
    <property name="spacing">20</property>
    <child>
      <object class="GtkPaned">
        <property name="orientation">vertical</property>
        <child>
          <object class="GtkBox">
            <property name="orientation">vertical</property>
            <property name="spacing">15</property>
            <child>
              <object class="GtkLabel">
                <property name="halign">start</property>
                <property name="label">Validadores</property>
                <style>
                  <class name="title-1"/>
                </style>
              </object>
            </child>
            <child>
              <object class="GtkScrolledWindow">
                <property name="vexpand">True</property>
                <child>
                  <object class="GtkBox">
                    <property name="homogeneous">True</property>
                    <property name="spacing">10</property>
                    <child>
                      <object class="GtkBox" id="box_detalles_validador">
                        <property name="margin-bottom">10</property>
                        <property name="margin-top">10</property>
                        <property name="orientation">vertical</property>
                        <property name="spacing">10</property>
                        <child>
                          <object class="GtkListBox" id="list_configurable">
                            <property name="activate-on-single-click">False</property>
                            <property name="selection-mode">none</property>
                            <child>
                              <object class="GtkListBoxRow">
                                <property name="child">
                                  <object class="GtkLabel" id="lbl_titulo">
                                    <property name="label">Sin Definir</property>
                                    <style>
                                      <class name="title-3"/>
                                    </style>
                                  </object>
                                </property>
                              </object>
                            </child>
                            <child>
                              <object class="GtkListBoxRow">
                                <property name="child">
                                  <object class="GtkImage" id="img_validador">
                                    <property name="icon-name">dialog-question-symbolic</property>
                                    <property name="margin-bottom">10</property>
                                    <property name="margin-end">10</property>
                                    <property name="margin-start">10</property>
                                    <property name="margin-top">10</property>
                                    <property name="pixel-size">125</property>
                                  </object>
                                </property>
                              </object>
                            </child>
                            <style>
                              <class name="rich-list"/>
                              <class name="boxed-list"/>
                            </style>
                          </object>
                        </child>
                        <child>
                          <object class="GtkListBox" id="list_detalles">
                            <property name="activate-on-single-click">False</property>
                            <property name="selection-mode">none</property>
                            <child>
                              <object class="GtkListBoxRow">
                                <child>
                                  <object class="GtkBox">
                                    <child>
                                      <object class="GtkLabel">
                                        <property name="halign">start</property>
                                        <property name="hexpand">true</property>
                                        <property name="label">Puerto</property>
                                        <property name="valign">center</property>
                                        <property name="xalign">0</property>
                                        <style>
                                          <class name="title-5"/>
                                        </style>
                                      </object>
                                    </child>
                                    <child>
                                      <object class="GtkLabel" id="lbl_data_">
                                        <property name="halign">end</property>
                                        <property name="hexpand">true</property>
                                        <property name="label">--</property>
                                        <property name="valign">center</property>
                                        <property name="xalign">0</property>
                                        <style>
                                          <class name="dim-label"/>
                                        </style>
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
                                        <property name="label">ID</property>
                                        <property name="valign">center</property>
                                        <property name="xalign">0</property>
                                        <style>
                                          <class name="title-5"/>
                                        </style>
                                      </object>
                                    </child>
                                    <child>
                                      <object class="GtkLabel" id="lbl_data_0">
                                        <property name="halign">end</property>
                                        <property name="hexpand">true</property>
                                        <property name="label">--</property>
                                        <property name="valign">center</property>
                                        <property name="xalign">0</property>
                                        <style>
                                          <class name="dim-label"/>
                                        </style>
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
                                        <property name="label">Conectado</property>
                                        <property name="valign">center</property>
                                        <property name="xalign">0</property>
                                        <style>
                                          <class name="title-5"/>
                                        </style>
                                      </object>
                                    </child>
                                    <child>
                                      <object class="GtkLabel" id="lbl_data_1">
                                        <property name="halign">end</property>
                                        <property name="hexpand">true</property>
                                        <property name="label">--</property>
                                        <property name="valign">center</property>
                                        <property name="xalign">0</property>
                                        <style>
                                          <class name="dim-label"/>
                                        </style>
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
                                        <property name="label">Ver. SSP</property>
                                        <property name="valign">center</property>
                                        <property name="xalign">0</property>
                                        <style>
                                          <class name="title-5"/>
                                        </style>
                                      </object>
                                    </child>
                                    <child>
                                      <object class="GtkLabel" id="lbl_data_2">
                                        <property name="halign">end</property>
                                        <property name="hexpand">true</property>
                                        <property name="label">--</property>
                                        <property name="valign">center</property>
                                        <property name="xalign">0</property>
                                        <style>
                                          <class name="dim-label"/>
                                        </style>
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
                                        <property name="label">Error</property>
                                        <property name="valign">center</property>
                                        <property name="xalign">0</property>
                                        <style>
                                          <class name="title-5"/>
                                        </style>
                                      </object>
                                    </child>
                                    <child>
                                      <object class="GtkLabel" id="lbl_data_3">
                                        <property name="halign">end</property>
                                        <property name="hexpand">true</property>
                                        <property name="label">--</property>
                                        <property name="valign">center</property>
                                        <property name="xalign">0</property>
                                        <style>
                                          <class name="dim-label"/>
                                        </style>
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
                                        <property name="label">Firmware</property>
                                        <property name="valign">center</property>
                                        <property name="xalign">0</property>
                                        <style>
                                          <class name="title-5"/>
                                        </style>
                                      </object>
                                    </child>
                                    <child>
                                      <object class="GtkLabel" id="lbl_data_4">
                                        <property name="halign">end</property>
                                        <property name="hexpand">true</property>
                                        <property name="label">--</property>
                                        <property name="valign">center</property>
                                        <property name="xalign">0</property>
                                        <style>
                                          <class name="dim-label"/>
                                        </style>
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
                                        <property name="label">Set Regional</property>
                                        <property name="valign">center</property>
                                        <property name="xalign">0</property>
                                        <style>
                                          <class name="title-5"/>
                                        </style>
                                      </object>
                                    </child>
                                    <child>
                                      <object class="GtkLabel" id="lbl_data_5">
                                        <property name="halign">end</property>
                                        <property name="hexpand">true</property>
                                        <property name="label">--</property>
                                        <property name="valign">center</property>
                                        <property name="xalign">0</property>
                                        <style>
                                          <class name="dim-label"/>
                                        </style>
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
                                        <property name="label">ID Serial</property>
                                        <property name="valign">center</property>
                                        <property name="xalign">0</property>
                                        <style>
                                          <class name="title-5"/>
                                        </style>
                                      </object>
                                    </child>
                                    <child>
                                      <object class="GtkLabel" id="lbl_data_6">
                                        <property name="halign">end</property>
                                        <property name="hexpand">true</property>
                                        <property name="label">--</property>
                                        <property name="valign">center</property>
                                        <property name="xalign">0</property>
                                        <style>
                                          <class name="dim-label"/>
                                        </style>
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
                                        <property name="label">ID Modulo</property>
                                        <property name="valign">center</property>
                                        <property name="xalign">0</property>
                                        <style>
                                          <class name="title-5"/>
                                        </style>
                                      </object>
                                    </child>
                                    <child>
                                      <object class="GtkLabel" id="lbl_data_7">
                                        <property name="halign">end</property>
                                        <property name="hexpand">true</property>
                                        <property name="label">--</property>
                                        <property name="valign">center</property>
                                        <property name="xalign">0</property>
                                        <style>
                                          <class name="dim-label"/>
                                        </style>
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
                                        <property name="label">Revision</property>
                                        <property name="valign">center</property>
                                        <property name="xalign">0</property>
                                        <style>
                                          <class name="title-5"/>
                                        </style>
                                      </object>
                                    </child>
                                    <child>
                                      <object class="GtkLabel" id="lbl_data_8">
                                        <property name="halign">end</property>
                                        <property name="hexpand">true</property>
                                        <property name="label">--</property>
                                        <property name="valign">center</property>
                                        <property name="xalign">0</property>
                                        <style>
                                          <class name="dim-label"/>
                                        </style>
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
                                        <property name="label">Revision Modulo</property>
                                        <property name="valign">center</property>
                                        <property name="xalign">0</property>
                                        <style>
                                          <class name="title-5"/>
                                        </style>
                                      </object>
                                    </child>
                                    <child>
                                      <object class="GtkLabel" id="lbl_data_9">
                                        <property name="halign">end</property>
                                        <property name="hexpand">true</property>
                                        <property name="label">--</property>
                                        <property name="valign">center</property>
                                        <property name="xalign">0</property>
                                        <style>
                                          <class name="dim-label"/>
                                        </style>
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
                                        <property name="label">Reloj Interno</property>
                                        <property name="valign">center</property>
                                        <property name="xalign">0</property>
                                        <style>
                                          <class name="title-5"/>
                                        </style>
                                      </object>
                                    </child>
                                    <child>
                                      <object class="GtkLabel" id="lbl_data_10">
                                        <property name="ellipsize">start</property>
                                        <property name="halign">end</property>
                                        <property name="hexpand">true</property>
                                        <property name="label">--</property>
                                        <property name="valign">center</property>
                                        <property name="xalign">0</property>
                                        <style>
                                          <class name="dim-label"/>
                                        </style>
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
                    <child>
                      <object class="GtkBox" id="box_detalles_validador_2">
                        <property name="margin-bottom">10</property>
                        <property name="margin-top">10</property>
                        <property name="orientation">vertical</property>
                        <property name="spacing">10</property>
                        <child>
                          <object class="GtkListBox" id="list_configurable_2">
                            <property name="activate-on-single-click">False</property>
                            <property name="selection-mode">none</property>
                            <child>
                              <object class="GtkListBoxRow">
                                <property name="child">
                                  <object class="GtkLabel" id="lbl_titulo_">
                                    <property name="label">Sin Definir</property>
                                    <style>
                                      <class name="title-3"/>
                                    </style>
                                  </object>
                                </property>
                              </object>
                            </child>
                            <child>
                              <object class="GtkListBoxRow">
                                <property name="child">
                                  <object class="GtkImage" id="img_validador_">
                                    <property name="icon-name">dialog-question-symbolic</property>
                                    <property name="margin-bottom">10</property>
                                    <property name="margin-end">10</property>
                                    <property name="margin-start">10</property>
                                    <property name="margin-top">10</property>
                                    <property name="pixel-size">125</property>
                                  </object>
                                </property>
                              </object>
                            </child>
                            <style>
                              <class name="rich-list"/>
                              <class name="boxed-list"/>
                            </style>
                          </object>
                        </child>
                        <child>
                          <object class="GtkListBox" id="list_detalles_2">
                            <property name="activate-on-single-click">False</property>
                            <property name="selection-mode">none</property>
                            <child>
                              <object class="GtkListBoxRow">
                                <child>
                                  <object class="GtkBox">
                                    <child>
                                      <object class="GtkLabel">
                                        <property name="halign">start</property>
                                        <property name="hexpand">true</property>
                                        <property name="label">Puerto</property>
                                        <property name="valign">center</property>
                                        <property name="xalign">0</property>
                                        <style>
                                          <class name="title-5"/>
                                        </style>
                                      </object>
                                    </child>
                                    <child>
                                      <object class="GtkLabel" id="lbl_data__">
                                        <property name="halign">end</property>
                                        <property name="hexpand">true</property>
                                        <property name="label">--</property>
                                        <property name="valign">center</property>
                                        <property name="xalign">0</property>
                                        <style>
                                          <class name="dim-label"/>
                                        </style>
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
                                        <property name="label">ID</property>
                                        <property name="valign">center</property>
                                        <property name="xalign">0</property>
                                        <style>
                                          <class name="title-5"/>
                                        </style>
                                      </object>
                                    </child>
                                    <child>
                                      <object class="GtkLabel" id="lbl_data_0_">
                                        <property name="halign">end</property>
                                        <property name="hexpand">true</property>
                                        <property name="label">--</property>
                                        <property name="valign">center</property>
                                        <property name="xalign">0</property>
                                        <style>
                                          <class name="dim-label"/>
                                        </style>
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
                                        <property name="label">Conectado</property>
                                        <property name="valign">center</property>
                                        <property name="xalign">0</property>
                                        <style>
                                          <class name="title-5"/>
                                        </style>
                                      </object>
                                    </child>
                                    <child>
                                      <object class="GtkLabel" id="lbl_data_1_">
                                        <property name="halign">end</property>
                                        <property name="hexpand">true</property>
                                        <property name="label">--</property>
                                        <property name="valign">center</property>
                                        <property name="xalign">0</property>
                                        <style>
                                          <class name="dim-label"/>
                                        </style>
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
                                        <property name="label">Ver. SSP</property>
                                        <property name="valign">center</property>
                                        <property name="xalign">0</property>
                                        <style>
                                          <class name="title-5"/>
                                        </style>
                                      </object>
                                    </child>
                                    <child>
                                      <object class="GtkLabel" id="lbl_data_2_">
                                        <property name="halign">end</property>
                                        <property name="hexpand">true</property>
                                        <property name="label">--</property>
                                        <property name="valign">center</property>
                                        <property name="xalign">0</property>
                                        <style>
                                          <class name="dim-label"/>
                                        </style>
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
                                        <property name="label">Error</property>
                                        <property name="valign">center</property>
                                        <property name="xalign">0</property>
                                        <style>
                                          <class name="title-5"/>
                                        </style>
                                      </object>
                                    </child>
                                    <child>
                                      <object class="GtkLabel" id="lbl_data_3_">
                                        <property name="halign">end</property>
                                        <property name="hexpand">true</property>
                                        <property name="label">--</property>
                                        <property name="valign">center</property>
                                        <property name="xalign">0</property>
                                        <style>
                                          <class name="dim-label"/>
                                        </style>
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
                                        <property name="label">Firmware</property>
                                        <property name="valign">center</property>
                                        <property name="xalign">0</property>
                                        <style>
                                          <class name="title-5"/>
                                        </style>
                                      </object>
                                    </child>
                                    <child>
                                      <object class="GtkLabel" id="lbl_data_4_">
                                        <property name="halign">end</property>
                                        <property name="hexpand">true</property>
                                        <property name="label">--</property>
                                        <property name="valign">center</property>
                                        <property name="xalign">0</property>
                                        <style>
                                          <class name="dim-label"/>
                                        </style>
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
                                        <property name="label">Set Regional</property>
                                        <property name="valign">center</property>
                                        <property name="xalign">0</property>
                                        <style>
                                          <class name="title-5"/>
                                        </style>
                                      </object>
                                    </child>
                                    <child>
                                      <object class="GtkLabel" id="lbl_data_5_">
                                        <property name="halign">end</property>
                                        <property name="hexpand">true</property>
                                        <property name="label">--</property>
                                        <property name="valign">center</property>
                                        <property name="xalign">0</property>
                                        <style>
                                          <class name="dim-label"/>
                                        </style>
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
                                        <property name="label">ID Serial</property>
                                        <property name="valign">center</property>
                                        <property name="xalign">0</property>
                                        <style>
                                          <class name="title-5"/>
                                        </style>
                                      </object>
                                    </child>
                                    <child>
                                      <object class="GtkLabel" id="lbl_data_6_">
                                        <property name="halign">end</property>
                                        <property name="hexpand">true</property>
                                        <property name="label">--</property>
                                        <property name="valign">center</property>
                                        <property name="xalign">0</property>
                                        <style>
                                          <class name="dim-label"/>
                                        </style>
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
                                        <property name="label">ID Modulo</property>
                                        <property name="valign">center</property>
                                        <property name="xalign">0</property>
                                        <style>
                                          <class name="title-5"/>
                                        </style>
                                      </object>
                                    </child>
                                    <child>
                                      <object class="GtkLabel" id="lbl_data_7_">
                                        <property name="halign">end</property>
                                        <property name="hexpand">true</property>
                                        <property name="label">--</property>
                                        <property name="valign">center</property>
                                        <property name="xalign">0</property>
                                        <style>
                                          <class name="dim-label"/>
                                        </style>
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
                                        <property name="label">Revision</property>
                                        <property name="valign">center</property>
                                        <property name="xalign">0</property>
                                        <style>
                                          <class name="title-5"/>
                                        </style>
                                      </object>
                                    </child>
                                    <child>
                                      <object class="GtkLabel" id="lbl_data_8_">
                                        <property name="halign">end</property>
                                        <property name="hexpand">true</property>
                                        <property name="label">--</property>
                                        <property name="valign">center</property>
                                        <property name="xalign">0</property>
                                        <style>
                                          <class name="dim-label"/>
                                        </style>
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
                                        <property name="label">Revision Modulo</property>
                                        <property name="valign">center</property>
                                        <property name="xalign">0</property>
                                        <style>
                                          <class name="title-5"/>
                                        </style>
                                      </object>
                                    </child>
                                    <child>
                                      <object class="GtkLabel" id="lbl_data_9_">
                                        <property name="halign">end</property>
                                        <property name="hexpand">true</property>
                                        <property name="label">--</property>
                                        <property name="valign">center</property>
                                        <property name="xalign">0</property>
                                        <style>
                                          <class name="dim-label"/>
                                        </style>
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
                                        <property name="label">Reloj Interno</property>
                                        <property name="valign">center</property>
                                        <property name="xalign">0</property>
                                        <style>
                                          <class name="title-5"/>
                                        </style>
                                      </object>
                                    </child>
                                    <child>
                                      <object class="GtkLabel" id="lbl_data_10_">
                                        <property name="ellipsize">start</property>
                                        <property name="halign">end</property>
                                        <property name="hexpand">true</property>
                                        <property name="label">--</property>
                                        <property name="valign">center</property>
                                        <property name="xalign">0</property>
                                        <style>
                                          <class name="dim-label"/>
                                        </style>
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
                </child>
              </object>
            </child>
          </object>
        </child>
        <child>
          <object class="GtkBox">
            <property name="orientation">vertical</property>
            <property name="spacing">20</property>
            <child>
              <object class="GtkLabel">
                <property name="halign">start</property>
                <property name="label">Sistema</property>
                <style>
                  <class name="title-1"/>
                </style>
              </object>
            </child>
            <child>
              <object class="GtkListBox" id="list_info_systema">
                <child>
                  <object class="GtkListBoxRow">
                    <property name="activatable">false</property>
                    <child>
                      <object class="GtkBox">
                        <child>
                          <object class="GtkLabel">
                            <property name="halign">start</property>
                            <property name="label">Nombre del Dispositivo</property>
                          </object>
                        </child>
                        <child>
                          <object class="GtkEditableLabel" id="ety_info_host">
                            <property name="editable">true</property>
                            <property name="halign">end</property>
                            <property name="hexpand">true</property>
                            <property name="text">Type here…</property>
                            <style>
                              <class name="dim-label"/>
                            </style>
                          </object>
                        </child>
                      </object>
                    </child>
                  </object>
                </child>
                <child>
                  <object class="GtkListBoxRow">
                    <property name="activatable">false</property>
                    <child>
                      <object class="GtkBox">
                        <child>
                          <object class="GtkLabel" id="entry_">
                            <property name="halign">start</property>
                            <property name="label">Modelo de Hardware</property>
                          </object>
                        </child>
                        <child>
                          <object class="GtkEditableLabel" id="ety_info_modelo">
                            <property name="editable">false</property>
                            <property name="halign">end</property>
                            <property name="hexpand">true</property>
                            <property name="text">Type here…</property>
                            <style>
                              <class name="dim-label"/>
                            </style>
                          </object>
                        </child>
                      </object>
                    </child>
                  </object>
                </child>
                <child>
                  <object class="GtkListBoxRow">
                    <property name="activatable">false</property>
                    <child>
                      <object class="GtkBox">
                        <child>
                          <object class="GtkLabel">
                            <property name="halign">start</property>
                            <property name="label">Procesador</property>
                          </object>
                        </child>
                        <child>
                          <object class="GtkEditableLabel" id="ety_info_procesador">
                            <property name="editable">false</property>
                            <property name="halign">end</property>
                            <property name="hexpand">true</property>
                            <property name="text">Type here…</property>
                            <style>
                              <class name="dim-label"/>
                            </style>
                          </object>
                        </child>
                      </object>
                    </child>
                  </object>
                </child>
                <child>
                  <object class="GtkListBoxRow">
                    <property name="activatable">false</property>
                    <child>
                      <object class="GtkBox">
                        <child>
                          <object class="GtkLabel">
                            <property name="halign">start</property>
                            <property name="label">Memoria</property>
                          </object>
                        </child>
                        <child>
                          <object class="GtkEditableLabel" id="ety_info_memoria">
                            <property name="editable">false</property>
                            <property name="halign">end</property>
                            <property name="hexpand">true</property>
                            <property name="text">Type here…</property>
                            <style>
                              <class name="dim-label"/>
                            </style>
                          </object>
                        </child>
                      </object>
                    </child>
                  </object>
                </child>
                <child>
                  <object class="GtkListBoxRow">
                    <property name="activatable">false</property>
                    <child>
                      <object class="GtkBox">
                        <child>
                          <object class="GtkLabel">
                            <property name="halign">start</property>
                            <property name="label">Disco</property>
                          </object>
                        </child>
                        <child>
                          <object class="GtkEditableLabel" id="ety_info_disco">
                            <property name="editable">false</property>
                            <property name="halign">end</property>
                            <property name="hexpand">true</property>
                            <property name="text">Type here…</property>
                            <style>
                              <class name="dim-label"/>
                            </style>
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
              <object class="GtkListBox" id="list_acciones">
                <child>
                  <object class="GtkListBoxRow">
                    <child>
                      <object class="GtkBox">
                        <child>
                          <object class="GtkLabel">
                            <property name="halign">start</property>
                            <property name="hexpand">true</property>
                            <property name="label">Reiniciar</property>
                            <property name="valign">center</property>
                            <property name="xalign">0</property>
                          </object>
                        </child>
                        <child>
                          <object class="GtkImage">
                            <property name="halign">end</property>
                            <property name="icon-name">system-reboot-symbolic</property>
                            <property name="margin-end">10</property>
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
                            <property name="label">Apagar</property>
                            <property name="valign">center</property>
                            <property name="xalign">0</property>
                          </object>
                        </child>
                        <child>
                          <object class="GtkImage">
                            <property name="halign">end</property>
                            <property name="icon-name">system-shutdown-symbolic</property>
                            <property name="margin-end">10</property>
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
    </child>
  </object>
</interface>
)";
    } // namespace Conf
} // namespace View