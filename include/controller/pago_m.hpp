#pragma once
#include "view/pago_m.hpp"

class PagoM : public VPagoM
{
private:
    const std::map<int, int> coins = {{0, 1}, {1, 2}, {2, 5}, {3, 10}};
    const std::map<int, int> bills = {{0, 20}, {1, 50}, {2, 100}, {3, 200}, {4, 500}, {5, 1000}};
    size_t total;

    void on_spin_value_changed();
    void on_btn_cobrar_clicked();
public:
    PagoM(BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &refBuilder);
    ~PagoM();
};