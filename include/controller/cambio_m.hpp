#pragma once
#include "view/pago_m.hpp"
#include "model/log.hpp"
#include "controller/configuracion/impresora.hpp"
#include "global.hpp"

class CambioM : public VPagoM
{
private:
    int ingreso = 0, total = 0;
    const std::map<int, int> coins = {{0, 1}, {1, 2}, {2, 5}, {3, 10}};
    const std::map<int, int> bills = {{0, 20}, {1, 50}, {2, 100}, {3, 200}, {4, 500}, {5, 1000}};

    Gtk::Box v_box_botones{Gtk::Orientation::HORIZONTAL, 5};
    Gtk::Button v_btn_aceptar, v_btn_cancelar;

    void activa_spinners(Gtk::SpinButton *spinner[], int pos, int tam);
    void on_spin_value_changed();

    void on_show_map();

    void on_btn_cobrar_clicked();
    void on_btn_aceptar_clicked();
    void on_btn_cancelar_clicked();

public:
    CambioM(BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &refBuilder);
    ~CambioM();
};