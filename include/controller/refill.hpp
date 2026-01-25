#pragma once
#include "view/refill.hpp"
#include "controller/configuracion/impresora.hpp"


class Refill : public VRefill
{
private:
    void on_show_map();
    void init_data(Gtk::ColumnView *vcolumn,const Glib::RefPtr<Gio::ListStore<MLevelCash>>& level);
    void actualiza_data(const Glib::RefPtr<Gtk::SelectionModel> &selection,const Glib::RefPtr<Gio::ListStore<MLevelCash>>& level);
    void safe_clear_column_view(Gtk::ColumnView* column_view);

    void on_btn_iniciar();
    void on_btn_detener();
    void on_btn_transpaso();

    void poll_alerta_niveles();
    void alerta_niveles(const nlohmann::json &json);
    
public: 
    Refill(BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &refBuilder);
    ~Refill();
};