#pragma once
#include "view/movimientos.hpp"

class Movimientos : public VMovimientos
{
private:
    void init_datos();
    Glib::RefPtr<Gtk::StringList> m_list_tipos;
    void actualiza_data(const Glib::RefPtr<Gtk::SelectionModel> &selection, const Glib::RefPtr<Gio::ListStore<MLog>> &log);
    void consume_data();
public:
    Movimientos(BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &refBuilder);
    ~Movimientos();
};