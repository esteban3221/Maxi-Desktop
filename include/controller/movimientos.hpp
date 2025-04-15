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

    void init_datos();
    void reimprime_tickets();

    void actualiza_data(const Glib::RefPtr<Gtk::SelectionModel> &selection, const Glib::RefPtr<Gio::ListStore<MLog>> &log);
    void consume_data();
    void muestra_calendario_inicio(Gtk::Entry::IconPosition);
    void muestra_calendario_fin(Gtk::Entry::IconPosition);
    void set_fecha();
    void borra_filtro();

public:
    Movimientos(BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &refBuilder);
    ~Movimientos();
};