#pragma once

#include "view/movimientos.hpp"
#include "model/log.hpp"
#include "controller/configuracion/impresora.hpp"

class Movimientos : public VMovimientos
{
private:
    Glib::RefPtr<Gtk::StringList> m_list_tipos;
    Glib::RefPtr<Gtk::NumericSorter<unsigned int>> m_IdSorter;
    Glib::RefPtr<Gtk::NumericSorter<Glib::ustring::size_type>> m_LengthSorter;

    Glib::RefPtr<Gtk::ColumnViewColumn> column_id;
    Glib::RefPtr<Gtk::SortListModel> sorter_model;

    Glib::RefPtr<Gio::ListModel> create_model(const Glib::RefPtr<Glib::ObjectBase> &log = {});
    Glib::RefPtr<Gtk::TreeListModel> tree_model_list;
    // Glib::RefPtr<Gio::ListStore<MLog>> m_log;

    std::unique_ptr<Gtk::MessageDialog> v_dialog;

    void init_datos();
    void reimprime_tickets();
    void imprime_corte();

    const nlohmann::json &consume_data();
    nlohmann::json m_cached_data;  // Cache de los datos
    bool m_data_cached = false;     // Flag para saber si ya tenemos datos

    void init_refrescar_datos();
    void muestra_calendario_inicio(Gtk::Entry::IconPosition);
    void muestra_calendario_fin(Gtk::Entry::IconPosition);
    void set_fecha();
    void borra_filtro();

    //menu contextual
    void create_context_menu();
    void on_column_view_button_pressed(int n_press, double x, double y);
    void show_context_menu(double x, double y, const Glib::RefPtr<Glib::ObjectBase>& item);
    void on_menu_reimprimir();
    void on_menu_ver_detalles();
    void on_menu_anular();

    Glib::RefPtr<Gio::MenuItem> m_menu_item_reimprimir;
    Glib::RefPtr<Gio::MenuItem> m_menu_item_detalles;
    Glib::RefPtr<Gio::MenuItem> m_menu_item_anular;
    Gtk::PopoverMenu *m_popover_menu;

public:
    Movimientos(BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &refBuilder);
    ~Movimientos();
};