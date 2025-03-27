#pragma once
#include "view/pago_a.hpp"

class PagoA : public VPago
{
private:
    void on_btn_enter_clicked();

public:
    PagoA(/* args */);
    ~PagoA();
};