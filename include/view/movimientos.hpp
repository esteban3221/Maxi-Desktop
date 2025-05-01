#pragma once
#include <gtkmm.h>
#include "global.hpp"
#include "model/log.hpp"

class VMovimientos : public Gtk::Box
{
private:
    Glib::RefPtr<Gtk::Builder> m_builder;
protected:

    Gtk::Button *v_btn_printer, *v_btn_aplica_filtro, *v_btn_remueve_filtros;
    Gtk::DropDown *v_dp_tipo;
    Gtk::Entry *v_ety_ini,*v_ety_fin, *v_ety_pag;
    Gtk::SpinButton *v_spin_pag;

    Gtk::ColumnView *v_column_log;
    Gtk::Label *v_lbl_total_registros;

    Gtk::Popover v_pop_calendario;
    Gtk::Calendar v_calendario;


    void on_setup_label(const Glib::RefPtr<Gtk::ListItem> &list_item);
    void on_setup_label_text(const Glib::RefPtr<Gtk::ListItem> &list_item);
    
    void on_bind_id(const Glib::RefPtr<Gtk::ListItem> &list_item);
    void on_bind_usuario(const Glib::RefPtr<Gtk::ListItem> &list_item);
    void on_bind_tipo(const Glib::RefPtr<Gtk::ListItem> &list_item);
    void on_bind_ingreso(const Glib::RefPtr<Gtk::ListItem> &list_item);
    void on_bind_cambio(const Glib::RefPtr<Gtk::ListItem> &list_item);
    void on_bind_total(const Glib::RefPtr<Gtk::ListItem> &list_item);
    void on_bind_estatus(const Glib::RefPtr<Gtk::ListItem> &list_item);
    void on_bind_fecha(const Glib::RefPtr<Gtk::ListItem> &list_item);

public:
    VMovimientos(BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &refBuilder);
    ~VMovimientos();
};

namespace View
{
    extern const char *movimientos_ui;
} // namespace View