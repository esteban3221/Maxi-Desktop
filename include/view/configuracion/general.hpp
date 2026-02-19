#pragma once
#include <gtkmm.h>
#include "global.hpp"

class VGeneral : public Gtk::Box
{
private:
    Glib::RefPtr<Gtk::Builder> m_builder;

protected:
    // Visualización
    Gtk::Switch*   v_sw_mostrar_notificaciones  {nullptr};
    Gtk::Entry*    v_ety_mensaje_inicio         {nullptr};
    Gtk::Button*   v_btn_seleccionar_icono      {nullptr};
    Gtk::Button*   v_btn_seleccionar_carpeta    {nullptr};
    Gtk::Switch*   v_sw_desactivar_carrousel    {nullptr};

    // Efectivo
    Gtk::Switch*   v_sw_redondeo                {nullptr};
    Gtk::Switch*   v_sw_terminar_operacion      {nullptr};

    // Tarjeta
    Gtk::DropDown* v_dd_iniciar_proceso         {nullptr};
    Gtk::Switch*   v_sw_diferir                 {nullptr};

    // Acciones / Mantenimiento
    Gtk::Button*   v_btn_reiniciar_validadores  {nullptr};
    Gtk::Button*   v_btn_actualizar_pos         {nullptr};
    Gtk::Button*   v_btn_retirada               {nullptr};

    std::unique_ptr<Gtk::MessageDialog> v_dialog;

public:
    VGeneral(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refBuilder);
    ~VGeneral();
};

namespace View
{
    namespace Conf
    {
        extern const char *general_ui;
    } // namespace Conf
} // namespace View