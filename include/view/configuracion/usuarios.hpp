#pragma once
#include <gtkmm.h>
#include "global.hpp"
#include "model/usuarios.hpp"
#include "model/usuarios_roles.hpp"

class VUsuarios : public Gtk::Box
{
private:
    Glib::RefPtr<Gtk::Builder> m_builder;

protected:
    // Lista de usuarios
    Gtk::ColumnView* v_treeview {nullptr};

    // Botones de acción
    Gtk::Button* v_btn_nuevo     {nullptr};
    Gtk::Button* v_btn_modificar {nullptr};
    Gtk::Button* v_btn_eliminar  {nullptr};

    // Permisos - Master
    Gtk::Switch* v_chk_todos_roles {nullptr};

    // Permisos individuales (mejor individual para claridad y evitar errores)
    Gtk::CheckButton* v_chk_venta              {nullptr};
    Gtk::CheckButton* v_chk_pago               {nullptr};
    Gtk::CheckButton* v_chk_carga              {nullptr};
    Gtk::CheckButton* v_chk_retirada           {nullptr};
    Gtk::CheckButton* v_chk_cambio_man         {nullptr};
    Gtk::CheckButton* v_chk_cambio_auto        {nullptr};
    Gtk::CheckButton* v_chk_ingresos           {nullptr};
    Gtk::CheckButton* v_chk_envio_casette      {nullptr};
    Gtk::CheckButton* v_chk_retirada_casette   {nullptr};
    Gtk::CheckButton* v_chk_consulta_efectivo  {nullptr};
    Gtk::CheckButton* v_chk_mov_pendientes     {nullptr};
    Gtk::CheckButton* v_chk_consulta_mov       {nullptr};
    Gtk::CheckButton* v_chk_cierre_falt        {nullptr};
    Gtk::CheckButton* v_chk_estadisticas       {nullptr};
    Gtk::CheckButton* v_chk_fianza             {nullptr};
    Gtk::CheckButton* v_chk_mostrar_report     {nullptr};
    Gtk::CheckButton* v_chk_configuracion      {nullptr};
    Gtk::CheckButton* v_chk_salir_windows      {nullptr};
    Gtk::CheckButton* v_chk_apagar_equipo      {nullptr};

    std::vector<Gtk::CheckButton*> v_chk_roles;

    // Botón guardar
    Gtk::Button* v_btn_guardar_roles {nullptr};

    std::unique_ptr<Gtk::MessageDialog> v_dialog;

public:
    VUsuarios(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refBuilder);
    ~VUsuarios();

    void on_setup_label(const Glib::RefPtr<Gtk::ListItem>& list_item);
    void on_setup_row(const Glib::RefPtr<Gtk::ListItem>& list_item);
};


namespace View
{
    namespace Conf
    {
        extern const char *usuario_ui;
    } // namespace Conf
} // namespace View