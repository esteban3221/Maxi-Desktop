#include "view/configuracion/usuarios.hpp"

VUsuarios::VUsuarios(BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &refBuilder) : Gtk::Box(cobject),


                                                                                              m_builder(refBuilder)
{
    v_treeview = m_builder->get_widget<Gtk::ColumnView>("tree_usuarios");

    v_btn_nuevo = m_builder->get_widget<Gtk::Button>("btn_new");
    v_btn_modificar = m_builder->get_widget<Gtk::Button>("btn_edit");
    v_btn_eliminar = m_builder->get_widget<Gtk::Button>("btn_del");

    // Permisos individuales
    v_chk_venta              = m_builder->get_widget<Gtk::CheckButton>("check_venta");
    v_chk_pago               = m_builder->get_widget<Gtk::CheckButton>("check_pago");
    v_chk_carga              = m_builder->get_widget<Gtk::CheckButton>("check_carga");
    v_chk_retirada           = m_builder->get_widget<Gtk::CheckButton>("check_retirada");
    v_chk_cambio_man         = m_builder->get_widget<Gtk::CheckButton>("check_cambio_man");
    v_chk_cambio_auto        = m_builder->get_widget<Gtk::CheckButton>("check_cambio_auto");
    v_chk_ingresos           = m_builder->get_widget<Gtk::CheckButton>("check_ingresos");
    v_chk_envio_casette      = m_builder->get_widget<Gtk::CheckButton>("check_envio_casette");
    v_chk_retirada_casette   = m_builder->get_widget<Gtk::CheckButton>("check_retirada_casette");
    v_chk_consulta_efectivo  = m_builder->get_widget<Gtk::CheckButton>("check_consulta_efectivo");
    v_chk_mov_pendientes     = m_builder->get_widget<Gtk::CheckButton>("check_mov_pendientes");
    v_chk_consulta_mov       = m_builder->get_widget<Gtk::CheckButton>("check_consulta_mov");
    v_chk_cierre_falt        = m_builder->get_widget<Gtk::CheckButton>("check_cierre_falt");
    v_chk_estadisticas       = m_builder->get_widget<Gtk::CheckButton>("check_estadisticas");
    v_chk_fianza             = m_builder->get_widget<Gtk::CheckButton>("check_fianza");
    v_chk_mostrar_report     = m_builder->get_widget<Gtk::CheckButton>("check_mostrar_report");
    v_chk_configuracion      = m_builder->get_widget<Gtk::CheckButton>("check_configuracion");
    v_chk_salir_windows      = m_builder->get_widget<Gtk::CheckButton>("check_salir_windows");
    v_chk_apagar_equipo      = m_builder->get_widget<Gtk::CheckButton>("check_apagar_equipo");

//     v_chk_todos_roles = {
//     v_chk_venta, v_chk_pago, v_chk_carga, v_chk_retirada,
//     v_chk_cambio_man, v_chk_cambio_auto, v_chk_ingresos,
//     v_chk_envio_casette, v_chk_retirada_casette, v_chk_consulta_efectivo,
//     v_chk_mov_pendientes, v_chk_consulta_mov, v_chk_cierre_falt,
//     v_chk_estadisticas, v_chk_fianza, v_chk_mostrar_report,
//     v_chk_configuracion, v_chk_salir_windows, v_chk_apagar_equipo
// };

    v_chk_todos_roles = m_builder->get_widget<Gtk::Switch>("check_todos");

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
        const char *usuario_ui = R"(
        <?xml version="1.0" encoding="UTF-8"?>
<interface>
  <requires lib="gtk" version="4.0"/>
  <object class="GtkBox" id="box_usuarios">
    <property name="orientation">1</property>
    <property name="spacing">24</property>
    <property name="margin-top">16</property>
    <property name="margin-bottom">16</property>
    <property name="margin-start">24</property>
    <property name="margin-end">24</property>
    <child>
      <object class="GtkBox" id="barra_superior">
        <property name="orientation">0</property>
        <property name="spacing">16</property>
        <property name="halign">0</property>
        <property name="margin-bottom">16</property>
        <child>
          <object class="GtkLabel">
            <property name="label" translatable="yes">Usuarios del sistema</property>
            <property name="halign">1</property>
            <style>
              <class name="title-1"/>
            </style>
            <property name="hexpand">true</property>
          </object>
        </child>
        <child>
          <object class="GtkBox" id="botones_accion">
            <property name="orientation">0</property>
            <property name="spacing">0</property>
            <style>
              <class name="linked"/>
            </style>
            <child>
              <object class="GtkButton" id="btn_new">
                <property name="icon-name">list-add-symbolic</property>
                <property name="tooltip-text" translatable="yes">Agregar nuevo usuario</property>
                <style>
                  <class name="circular"/>
                  <class name="suggested-action"/>
                </style>
              </object>
            </child>
            <child>
              <object class="GtkButton" id="btn_edit">
                <property name="icon-name">document-edit-symbolic</property>
                <property name="tooltip-text" translatable="yes">Editar usuario seleccionado</property>
                <style>
                  <class name="circular"/>
                </style>
              </object>
            </child>
            <child>
              <object class="GtkButton" id="btn_del">
                <property name="icon-name">list-remove-symbolic</property>
                <property name="tooltip-text" translatable="yes">Eliminar usuario seleccionado</property>
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
      <object class="GtkBox">
        <property name="spacing">10</property>
        <child>
          <object class="GtkScrolledWindow">
            <property name="hexpand">true</property>
            <property name="vexpand">true</property>
            <property name="margin-bottom">16</property>
            <child>
              <object class="GtkColumnView" id="tree_usuarios">
                <property name="enable-rubberband">true</property>
                <property name="show-row-separators">true</property>
                <property name="reorderable">false</property>
              </object>
            </child>
          </object>
        </child>
        <child>
          <object class="GtkFrame" id="frame_permisos">
            <property name="label" translatable="yes">Permisos del usuario seleccionado</property>
            <property name="label-xalign">0</property>
            <property name="margin-top">8</property>
            <child>
              <object class="GtkBox" id="contenedor_permisos">
                <property name="orientation">1</property>
                <property name="spacing">20</property>
                <property name="margin-top">16</property>
                <property name="margin-bottom">16</property>
                <property name="margin-start">16</property>
                <property name="margin-end">16</property>
                <child>
                  <object class="GtkBox">
                    <property name="spacing">15</property>
                    <property name="orientation">0</property>
                    <child>
                      <object class="GtkSwitch" id="check_todos">
                        <property name="halign">3</property>
                        <property name="valign">3</property>
                        <property name="active">false</property>
                      </object>
                    </child>
                    <child>
                      <object class="GtkLabel">
                        <property name="label" translatable="yes">Administrador</property>
                      </object>
                    </child>
                  </object>
                </child>
                <child>
                  <object class="GtkListBox" id="lista_permisos">
                    <property name="selection-mode">0</property>
                    <child>
                      <object class="GtkListBoxRow">
                        <property name="activatable">false</property>
                        <child>
                          <object class="GtkBox">
                            <property name="orientation">1</property>
                            <property name="spacing">12</property>
                            <property name="margin-top">12</property>
                            <property name="margin-bottom">12</property>
                            <child>
                              <object class="GtkLabel">
                                <property name="label" translatable="yes">Operaciones diarias</property>
                                <property name="halign">1</property>
                                <style>
                                  <class name="heading"/>
                                </style>
                              </object>
                            </child>
                            <child>
                              <object class="GtkBox" id="checks">
                                <property name="orientation">0</property>
                                <property name="spacing">32</property>
                                <property name="homogeneous">true</property>
                                <child>
                                  <object class="GtkBox">
                                    <property name="orientation">1</property>
                                    <property name="spacing">10</property>
                                    <child>
                                      <object class="GtkCheckButton" id="check_venta">
                                        <property name="label" translatable="yes">Venta</property>
                                      </object>
                                    </child>
                                    <child>
                                      <object class="GtkCheckButton" id="check_pago">
                                        <property name="label" translatable="yes">Pago</property>
                                      </object>
                                    </child>
                                    <child>
                                      <object class="GtkCheckButton" id="check_carga">
                                        <property name="label" translatable="yes">Carga</property>
                                      </object>
                                    </child>
                                    <child>
                                      <object class="GtkCheckButton" id="check_retirada">
                                        <property name="label" translatable="yes">Retirada</property>
                                      </object>
                                    </child>
                                  </object>
                                </child>
                                <child>
                                  <object class="GtkBox">
                                    <property name="orientation">1</property>
                                    <property name="spacing">10</property>
                                    <child>
                                      <object class="GtkCheckButton" id="check_cambio_man">
                                        <property name="label" translatable="yes">Cambio Manual</property>
                                      </object>
                                    </child>
                                    <child>
                                      <object class="GtkCheckButton" id="check_cambio_auto">
                                        <property name="label" translatable="yes">Cambio Automático</property>
                                      </object>
                                    </child>
                                    <child>
                                      <object class="GtkCheckButton" id="check_ingresos">
                                        <property name="label" translatable="yes">Ingresos</property>
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
                      <object class="GtkListBoxRow">
                        <property name="activatable">false</property>
                        <child>
                          <object class="GtkBox">
                            <property name="orientation">1</property>
                            <property name="spacing">12</property>
                            <property name="margin-top">12</property>
                            <property name="margin-bottom">12</property>
                            <child>
                              <object class="GtkLabel">
                                <property name="label" translatable="yes">Gestión de efectivo y consultas</property>
                                <property name="halign">1</property>
                                <style>
                                  <class name="heading"/>
                                </style>
                              </object>
                            </child>
                            <child>
                              <object class="GtkBox">
                                <property name="orientation">0</property>
                                <property name="spacing">32</property>
                                <property name="homogeneous">true</property>
                                <child>
                                  <object class="GtkBox">
                                    <property name="orientation">1</property>
                                    <property name="spacing">10</property>
                                    <child>
                                      <object class="GtkCheckButton" id="check_envio_casette">
                                        <property name="label" translatable="yes">Enviar a casette</property>
                                      </object>
                                    </child>
                                    <child>
                                      <object class="GtkCheckButton" id="check_retirada_casette">
                                        <property name="label" translatable="yes">Retirada Casette</property>
                                      </object>
                                    </child>
                                    <child>
                                      <object class="GtkCheckButton" id="check_consulta_efectivo">
                                        <property name="label" translatable="yes">Consulta de Efectivo</property>
                                      </object>
                                    </child>
                                  </object>
                                </child>
                                <child>
                                  <object class="GtkBox">
                                    <property name="orientation">1</property>
                                    <property name="spacing">10</property>
                                    <child>
                                      <object class="GtkCheckButton" id="check_mov_pendientes">
                                        <property name="label" translatable="yes">Mov. Pendientes</property>
                                      </object>
                                    </child>
                                    <child>
                                      <object class="GtkCheckButton" id="check_consulta_mov">
                                        <property name="label" translatable="yes">Consulta Movimiento</property>
                                      </object>
                                    </child>
                                    <child>
                                      <object class="GtkCheckButton" id="check_cierre_falt">
                                        <property name="label" translatable="yes">Cierre con Faltantes</property>
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
                      <object class="GtkListBoxRow">
                        <property name="activatable">false</property>
                        <child>
                          <object class="GtkBox">
                            <property name="orientation">1</property>
                            <property name="spacing">12</property>
                            <property name="margin-top">12</property>
                            <property name="margin-bottom">12</property>
                            <child>
                              <object class="GtkLabel">
                                <property name="label" translatable="yes">Reportes y configuración avanzada</property>
                                <property name="halign">1</property>
                                <style>
                                  <class name="heading"/>
                                </style>
                              </object>
                            </child>
                            <child>
                              <object class="GtkBox">
                                <property name="orientation">0</property>
                                <property name="spacing">32</property>
                                <property name="homogeneous">true</property>
                                <child>
                                  <object class="GtkBox">
                                    <property name="orientation">1</property>
                                    <property name="spacing">10</property>
                                    <child>
                                      <object class="GtkCheckButton" id="check_estadisticas">
                                        <property name="label" translatable="yes">Estadisticas</property>
                                      </object>
                                    </child>
                                    <child>
                                      <object class="GtkCheckButton" id="check_fianza">
                                        <property name="label" translatable="yes">Fianza</property>
                                      </object>
                                    </child>
                                    <child>
                                      <object class="GtkCheckButton" id="check_mostrar_report">
                                        <property name="label" translatable="yes">Mostrar Reportes</property>
                                      </object>
                                    </child>
                                  </object>
                                </child>
                                <child>
                                  <object class="GtkBox">
                                    <property name="orientation">1</property>
                                    <property name="spacing">10</property>
                                    <child>
                                      <object class="GtkCheckButton" id="check_configuracion">
                                        <property name="label" translatable="yes">Configuración</property>
                                      </object>
                                    </child>
                                    <child>
                                      <object class="GtkCheckButton" id="check_salir_windows">
                                        <property name="label" translatable="yes">Salir a Windows</property>
                                      </object>
                                    </child>
                                    <child>
                                      <object class="GtkCheckButton" id="check_apagar_equipo">
                                        <property name="label" translatable="yes">Apagar el equipo</property>
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
                  </object>
                </child>
                <child>
                  <object class="GtkButton" id="btn_guardar_permisos">
                    <property name="label" translatable="yes">Guardar permisos</property>
                    <property name="halign">2</property>
                    <style>
                      <class name="suggested-action"/>
                      <class name="pill"/>
                    </style>
                    <property name="margin-top">24</property>
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