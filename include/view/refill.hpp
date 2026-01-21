#pragma once
#include <gtkmm.h>
#include "global.hpp"
#include "model/level_cash.hpp"

class VRefill : public Gtk::Box
{
private:
    Glib::RefPtr<Gtk::Builder> m_builder;

protected:
    Gtk::Label *v_lbl_total_parcial_monedas,
        *v_lbl_total_parcial_billetes,
        *v_lbl_total,
        *v_lbl_total_billetes_cass,
        *v_lbl_total_billetes,
        *v_lbl_total_monedas;

    Gtk::ColumnView *v_tree_reciclador_monedas = nullptr;
    Gtk::ColumnView *v_tree_reciclador_billetes = nullptr;

    // me dio hueva iterar los 2 ultimas columnas de monedas y billetes, asi que las hice aparte

    Glib::RefPtr<Gtk::ColumnViewColumn> v_mon_col_ingreso = nullptr;
    Glib::RefPtr<Gtk::ColumnViewColumn> v_mon_col_ingreso_total = nullptr;

    Glib::RefPtr<Gtk::ColumnViewColumn> v_bill_col_ingreso = nullptr;
    Glib::RefPtr<Gtk::ColumnViewColumn> v_bill_col_ingreso_total = nullptr;

    Gtk::Button *v_btn_incia = nullptr;
    Gtk::Button *v_btn_transpaso = nullptr;
    Gtk::Button *v_btn_detener = nullptr;


    void on_setup_label(const Glib::RefPtr<Gtk::ListItem> &list_item);
    void on_setup_button(const Glib::RefPtr<Gtk::ListItem> &list_item);
    void on_setup_spin(const Glib::RefPtr<Gtk::ListItem> &list_item);
    void on_bind_deno(const Glib::RefPtr<Gtk::ListItem> &list_item);
    void on_bind_alm(const Glib::RefPtr<Gtk::ListItem> &list_item);
    void on_bind_recy(const Glib::RefPtr<Gtk::ListItem> &list_item);
    void on_bind_ingreso(const Glib::RefPtr<Gtk::ListItem> &list_item);
    void on_bind_ingreso_total(const Glib::RefPtr<Gtk::ListItem> &list_item);
    void on_bind_inmo_min(const Glib::RefPtr<Gtk::ListItem> &list_item);
    
    void on_bind_inmo(const Glib::RefPtr<Gtk::ListItem> &list_item);
    void on_bind_inmo_max(const Glib::RefPtr<Gtk::ListItem> &list_item);
    void on_bind_btn(const Glib::RefPtr<Gtk::ListItem> &list_item);

public:
    VRefill(BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &refBuilder);
    ~VRefill();
};

namespace View
{
    extern const char *refill_ui;
} // namespace View