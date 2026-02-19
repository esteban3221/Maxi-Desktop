#include "view/configuracion/general.hpp"

VGeneral::VGeneral(BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &refBuilder) : Gtk::Box(cobject),
                                                                                            m_builder(refBuilder)
{
    v_sw_mostrar_notificaciones = m_builder->get_widget<Gtk::Switch>("sw_mostrar_notificaciones");
    v_ety_mensaje_inicio = m_builder->get_widget<Gtk::Entry>("ety_mensaje_inicio");
    v_btn_seleccionar_icono = m_builder->get_widget<Gtk::Button>("btn_seleccionar_icono");
    v_btn_seleccionar_carpeta = m_builder->get_widget<Gtk::Button>("btn_seleccionar_carpeta");
    v_sw_desactivar_carrousel = m_builder->get_widget<Gtk::Switch>("sw_desactivar_carrousel");
    v_sw_redondeo = m_builder->get_widget<Gtk::Switch>("sw_redondeo");
    v_sw_terminar_operacion = m_builder->get_widget<Gtk::Switch>("sw_terminar_operacion");
    v_dd_iniciar_proceso = m_builder->get_widget<Gtk::DropDown>("dd_iniciar_proceso");
    v_sw_diferir = m_builder->get_widget<Gtk::Switch>("sw_diferir");
    v_btn_reiniciar_validadores = m_builder->get_widget<Gtk::Button>("btn_reiniciar_validadores");
    v_btn_actualizar_pos = m_builder->get_widget<Gtk::Button>("btn_actualizar_pos");
    v_btn_retirada = m_builder->get_widget<Gtk::Button>("btn_retirada");
}

VGeneral::~VGeneral()
{
}

namespace View
{
    namespace Conf
    {
        const char *general_ui = R"(<?xml version='1.0' encoding='UTF-8'?>
<!-- Created with Cambalache 0.96.3 -->
<interface>
  <!-- interface-name general.ui -->
  <requires lib="gtk" version="4.0"/>
  <object class="GtkBox" id="page_general">
    <child>
      <object class="GtkScrolledWindow">
        <property name="hexpand">True</property>
        <child>
          <object class="GtkBox">
            <property name="margin-bottom">16</property>
            <property name="margin-end">24</property>
            <property name="margin-start">24</property>
            <property name="margin-top">16</property>
            <property name="orientation">vertical</property>
            <child>
              <object class="GtkLabel">
                <property name="halign">start</property>
                <property name="label" translatable="yes">Visualización</property>
                <style>
                  <class name="title-2"/>
                </style>
              </object>
            </child>
            <child>
              <object class="GtkListBox" id="list_visual">
                <property name="margin-top">16</property>
                <property name="selection-mode">none</property>
                <child>
                  <object class="GtkListBoxRow">
                    <child>
                      <object class="GtkBox">
                        <property name="margin-bottom">8</property>
                        <property name="margin-end">12</property>
                        <property name="margin-start">12</property>
                        <property name="margin-top">8</property>
                        <property name="orientation">horizontal</property>
                        <property name="spacing">12</property>
                        <child>
                          <object class="GtkLabel">
                            <property name="halign">start</property>
                            <property name="hexpand">true</property>
                            <property name="label" translatable="yes">Mostrar notificaciones</property>
                          </object>
                        </child>
                        <child>
                          <object class="GtkSwitch" id="sw_mostrar_notificaciones">
                            <property name="active">true</property>
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
                        <property name="margin-bottom">8</property>
                        <property name="margin-end">12</property>
                        <property name="margin-start">12</property>
                        <property name="margin-top">8</property>
                        <property name="orientation">horizontal</property>
                        <property name="spacing">12</property>
                        <child>
                          <object class="GtkBox">
                            <property name="hexpand">true</property>
                            <property name="orientation">vertical</property>
                            <property name="spacing">2</property>
                            <child>
                              <object class="GtkLabel">
                                <property name="halign">start</property>
                                <property name="label" translatable="yes">Mensaje de inicio</property>
                              </object>
                            </child>
                            <child>
                              <object class="GtkLabel">
                                <property name="halign">start</property>
                                <property name="label" translatable="yes">Texto que aparece al encender el sistema</property>
                                <style>
                                  <class name="dim-label"/>
                                  <class name="caption"/>
                                </style>
                              </object>
                            </child>
                          </object>
                        </child>
                        <child>
                          <object class="GtkEntry" id="ety_mensaje_inicio">
                            <property name="placeholder-text" translatable="yes">Escribe el mensaje aquí...</property>
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
                        <property name="margin-bottom">8</property>
                        <property name="margin-end">12</property>
                        <property name="margin-start">12</property>
                        <property name="margin-top">8</property>
                        <property name="orientation">horizontal</property>
                        <property name="spacing">12</property>
                        <child>
                          <object class="GtkBox">
                            <property name="hexpand">true</property>
                            <property name="orientation">vertical</property>
                            <property name="spacing">2</property>
                            <child>
                              <object class="GtkLabel">
                                <property name="halign">start</property>
                                <property name="label" translatable="yes">Icono del POS</property>
                              </object>
                            </child>
                            <child>
                              <object class="GtkLabel">
                                <property name="halign">start</property>
                                <property name="label" translatable="yes">Imagen personalizada para el punto de venta</property>
                                <style>
                                  <class name="dim-label"/>
                                  <class name="caption"/>
                                </style>
                              </object>
                            </child>
                          </object>
                        </child>
                        <child>
                          <object class="GtkButton" id="btn_seleccionar_icono">
                            <property name="label" translatable="yes">Seleccionar imagen</property>
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
                        <property name="margin-bottom">8</property>
                        <property name="margin-end">12</property>
                        <property name="margin-start">12</property>
                        <property name="margin-top">8</property>
                        <property name="orientation">horizontal</property>
                        <property name="spacing">12</property>
                        <child>
                          <object class="GtkBox">
                            <property name="hexpand">true</property>
                            <property name="orientation">vertical</property>
                            <property name="spacing">2</property>
                            <child>
                              <object class="GtkLabel">
                                <property name="halign">start</property>
                                <property name="label" translatable="yes">Carrusel del POS</property>
                              </object>
                            </child>
                            <child>
                              <object class="GtkLabel">
                                <property name="halign">start</property>
                                <property name="label" translatable="yes">Carpeta con imágenes rotativas</property>
                                <style>
                                  <class name="dim-label"/>
                                  <class name="caption"/>
                                </style>
                              </object>
                            </child>
                          </object>
                        </child>
                        <child>
                          <object class="GtkButton" id="btn_seleccionar_carpeta">
                            <property name="label" translatable="yes">Seleccionar carpeta</property>
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
                        <property name="margin-bottom">8</property>
                        <property name="margin-end">12</property>
                        <property name="margin-start">12</property>
                        <property name="margin-top">8</property>
                        <property name="orientation">horizontal</property>
                        <property name="spacing">12</property>
                        <child>
                          <object class="GtkLabel">
                            <property name="halign">start</property>
                            <property name="hexpand">true</property>
                            <property name="label" translatable="yes">Desactivar carrusel POS</property>
                          </object>
                        </child>
                        <child>
                          <object class="GtkSwitch" id="sw_desactivar_carrousel">
                            <property name="valign">center</property>
                          </object>
                        </child>
                      </object>
                    </child>
                  </object>
                </child>
                <style>
                  <class name="boxed-list"/>
                  <class name="rich-list"/>
                </style>
              </object>
            </child>
            <child>
              <object class="GtkLabel">
                <property name="halign">start</property>
                <property name="label" translatable="yes">Operaciones en efectivo</property>
                <property name="margin-top">32</property>
                <style>
                  <class name="title-2"/>
                </style>
              </object>
            </child>
            <child>
              <object class="GtkListBox" id="list_efectivo">
                <property name="margin-top">16</property>
                <property name="selection-mode">none</property>
                <child>
                  <object class="GtkListBoxRow">
                    <child>
                      <object class="GtkBox">
                        <property name="orientation">horizontal</property>
                        <property name="spacing">12</property>
                        <child>
                          <object class="GtkBox">
                            <property name="hexpand">true</property>
                            <property name="orientation">vertical</property>
                            <property name="spacing">2</property>
                            <child>
                              <object class="GtkLabel">
                                <property name="halign">start</property>
                                <property name="label" translatable="yes">Redondear cambio a favor del cliente</property>
                              </object>
                            </child>
                            <child>
                              <object class="GtkLabel">
                                <property name="halign">start</property>
                                <property name="label" translatable="yes">Evita centavos sobrantes a favor del negocio</property>
                                <style>
                                  <class name="dim-label"/>
                                  <class name="caption"/>
                                </style>
                              </object>
                            </child>
                          </object>
                        </child>
                        <child>
                          <object class="GtkSwitch" id="sw_redondeo">
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
                        <property name="orientation">horizontal</property>
                        <property name="spacing">12</property>
                        <child>
                          <object class="GtkBox">
                            <property name="hexpand">true</property>
                            <property name="orientation">vertical</property>
                            <property name="spacing">2</property>
                            <child>
                              <object class="GtkLabel">
                                <property name="halign">start</property>
                                <property name="label" translatable="yes">Terminar cualquier operación</property>
                              </object>
                            </child>
                            <child>
                              <object class="GtkLabel">
                                <property name="halign">start</property>
                                <property name="label" translatable="yes">Permite cerrar ventas incompletas</property>
                                <style>
                                  <class name="dim-label"/>
                                  <class name="caption"/>
                                </style>
                              </object>
                            </child>
                          </object>
                        </child>
                        <child>
                          <object class="GtkSwitch" id="sw_terminar_operacion">
                            <property name="valign">center</property>
                          </object>
                        </child>
                      </object>
                    </child>
                  </object>
                </child>
                <style>
                  <class name="boxed-list"/>
                  <class name="rich-list"/>
                </style>
              </object>
            </child>
            <child>
              <object class="GtkLabel">
                <property name="halign">start</property>
                <property name="label" translatable="yes">Operaciones en tarjeta</property>
                <property name="margin-top">32</property>
                <style>
                  <class name="title-2"/>
                </style>
              </object>
            </child>
            <child>
              <object class="GtkLabel">
                <property name="halign">start</property>
                <property name="label" translatable="yes">Solo para terminales de Mercado Pago</property>
                <property name="margin-bottom">8</property>
                <style>
                  <class name="dim-label"/>
                  <class name="caption"/>
                </style>
              </object>
            </child>
            <child>
              <object class="GtkListBox" id="list_tarjeta">
                <property name="margin-top">16</property>
                <property name="selection-mode">none</property>
                <child>
                  <object class="GtkListBoxRow">
                    <child>
                      <object class="GtkBox">
                        <property name="orientation">horizontal</property>
                        <property name="spacing">12</property>
                        <child>
                          <object class="GtkBox">
                            <property name="hexpand">true</property>
                            <property name="orientation">vertical</property>
                            <property name="spacing">2</property>
                            <child>
                              <object class="GtkLabel">
                                <property name="halign">start</property>
                                <property name="label" translatable="yes">Iniciar proceso en</property>
                              </object>
                            </child>
                            <child>
                              <object class="GtkLabel">
                                <property name="halign">start</property>
                                <property name="label" translatable="yes">Modo predeterminado al cobrar con tarjeta</property>
                                <style>
                                  <class name="dim-label"/>
                                  <class name="caption"/>
                                </style>
                              </object>
                            </child>
                          </object>
                        </child>
                        <child>
                          <object class="GtkDropDown" id="dd_iniciar_proceso">
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
                        <property name="orientation">horizontal</property>
                        <property name="spacing">12</property>
                        <child>
                          <object class="GtkBox">
                            <property name="hexpand">true</property>
                            <property name="orientation">vertical</property>
                            <property name="spacing">2</property>
                            <child>
                              <object class="GtkLabel">
                                <property name="halign">start</property>
                                <property name="label" translatable="yes">Permite diferir venta</property>
                              </object>
                            </child>
                            <child>
                              <object class="GtkLabel">
                                <property name="halign">start</property>
                                <property name="label" translatable="yes">Pagar el total en varias tarjetas o plazos</property>
                                <style>
                                  <class name="dim-label"/>
                                  <class name="caption"/>
                                </style>
                              </object>
                            </child>
                          </object>
                        </child>
                        <child>
                          <object class="GtkSwitch" id="sw_diferir">
                            <property name="valign">center</property>
                          </object>
                        </child>
                      </object>
                    </child>
                  </object>
                </child>
                <style>
                  <class name="boxed-list"/>
                  <class name="rich-list"/>
                </style>
              </object>
            </child>
            <child>
              <object class="GtkLabel">
                <property name="halign">start</property>
                <property name="label" translatable="yes">Mantenimiento y validadores</property>
                <property name="margin-top">32</property>
                <style>
                  <class name="title-2"/>
                </style>
              </object>
            </child>
            <child>
              <object class="GtkBox" id="acciones">
                <property name="halign">start</property>
                <property name="homogeneous">true</property>
                <property name="margin-top">12</property>
                <property name="orientation">horizontal</property>
                <property name="spacing">16</property>
                <child>
                  <object class="GtkButton" id="btn_reiniciar_validadores">
                    <property name="label" translatable="yes">Reiniciar validadores</property>
                  </object>
                </child>
                <child>
                  <object class="GtkButton" id="btn_actualizar_pos">
                    <property name="label" translatable="yes">Actualizar sistema POS</property>
                  </object>
                </child>
                <child>
                  <object class="GtkButton" id="btn_retirada">
                    <property name="label" translatable="yes">Retirada de cassette</property>
                    <style>
                      <class name="destructive-action"/>
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
</interface>
)";
    } // namespace Conf
} // namespace View