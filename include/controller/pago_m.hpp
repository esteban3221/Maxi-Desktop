#pragma once
#include "view/pago_m.hpp"
#include "model/log.hpp"
#include "controller/configuracion/impresora.hpp"
#include "global.hpp"
#include <level_cash.hpp>
#include <utility>
#include <vector>

class PagoM : public VPagoM
{
private:
    size_t total;

    void on_spin_value_changed();
    void on_btn_cobrar_clicked();
    void on_show_map();
public:
    PagoM(BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &refBuilder);
    ~PagoM();
};