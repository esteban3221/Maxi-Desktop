#pragma once
#include "view/pago_a.hpp"
#include "model/log.hpp"
#include "controller/configuracion/impresora.hpp"

class PagoA : public VPago
{
private:
    void on_btn_enter_clicked();
    void on_map_show();

public:
    PagoA(/* args */);
    ~PagoA();
    
};