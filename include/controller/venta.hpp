#pragma once
#include "view/venta.hpp"

class Venta : public VVenta
{
private:
    void on_btn_enter_clicked();
    void on_map_show();
public:
    Venta(/* args */);
    ~Venta();
};