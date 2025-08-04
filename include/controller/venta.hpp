#pragma once
#include "view/venta.hpp"
#include "model/log.hpp"
#include "controller/configuracion/impresora.hpp"

class Venta : public VVenta
{
private:
    void on_btn_enter_clicked();
    void on_map_show();

    bool is_view_ingreso;
public:
    Venta(bool is_view_ingreso = false);
    ~Venta();
};